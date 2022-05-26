#include <Logger.h>
#include <LogFileSink.h>
#include <LogQueue.h>
#include <Windows/LogDebugOutputSink.h>
#include <LogUnbufferedFileSink.h>

#include <string>
#include <filesystem>

#include <gtest/gtest.h>
#include "TestUtils.h"
#include "FakeStringLogSink.h"
#include "StringUtils.h"
#include "MockLogSink.h"
#include <LogFormatter.h>

// Functional: use global instance g_LogCentral
TEST(Functional, DefaultBehaviour)
{
  // This is not a real test, more like a demonstration of the capabilities of the global logger.
  // - output in the Visual Studio output window
  // - no log file
  // - in debug build: min log level is Verbose
  // - in release build: min log level is Warning

  // Test that there are no log files in the output directory
  std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
  DirectoryEntries initialDirectoryEntries = GetDirectoryContents(testOuputDirectoryPath);

  LOGV("hello verbose");
  LOGD("hello debug");
  LOGD("hello debug in utf-8");
  LOGI("hello information in utf-8");
  LOGW("hello warning");
  LOGE("hello error");
  LOGD(true);
  LOGI(false);

  // Test that there are no log files in the output directory
  DirectoryEntries finalDirectoryEntries = GetDirectoryContents(testOuputDirectoryPath);
  EXPECT_EQ(initialDirectoryEntries, finalDirectoryEntries);
}

TEST(Logger, LogSinkInterface)
{
  Logger logger;

  std::shared_ptr<FakeStringLogSink> stringSink = std::make_shared<FakeStringLogSink>();
  logger.AddListener(stringSink);
  logger.SetFormatter(nullptr);

  MKL_LOGE(&logger, "test");

  std::string buffer = stringSink->Buffer;
  EXPECT_NE(std::string::npos, buffer.find("test"));
  EXPECT_EQ(5u, buffer.size()); // test + ENDLINE
}

TEST(Logger, LogLevelFilter)
{
  Logger localLogger;
  localLogger.AddListener(std::make_shared<FakeStringLogSink>());

  for (ELogLevel eSetLogLevel = ELogLevel::Min; eSetLogLevel <= ELogLevel::Max; ++eSetLogLevel)
  {
    localLogger.SetMinimumLogLevel(eSetLogLevel);

    for (ELogLevel eExpectLogLevel = ELogLevel::Min; eExpectLogLevel <= ELogLevel::Max; ++eExpectLogLevel)
    {
      if (eExpectLogLevel < eSetLogLevel)
      {
        EXPECT_FALSE(localLogger.IsLogged(eExpectLogLevel));
      }
      else
      {
        EXPECT_TRUE(localLogger.IsLogged(eExpectLogLevel));
      }
    }
  }
}

TEST(Logger, LogLevelFilterWithNoLogSink)
{
  Logger localLogger;

  for (ELogLevel eSetLogLevel = ELogLevel::Min; eSetLogLevel <= ELogLevel::Max; ++eSetLogLevel)
  {
    localLogger.SetMinimumLogLevel(eSetLogLevel);

    for (ELogLevel eExpectLogLevel = ELogLevel::Min; eExpectLogLevel <= ELogLevel::Max; ++eExpectLogLevel)
    {
      EXPECT_FALSE(localLogger.IsLogged(eExpectLogLevel));
    }
  }
}

TEST(Logger, Utf8)
{
  auto stringSink = std::make_shared<FakeStringLogSink>();
  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  logger.AddListener(stringSink);

  const char* szTestMessage = u8"Hello World\n你好世界";
  std::string testMessage = szTestMessage;

  MKL_LOGI(&logger, szTestMessage);
  std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(szTestMessage));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_STREQ(szTestMessage, actualMessage.c_str());

  stringSink->Buffer.clear();

  MKL_LOGI(&logger, testMessage);
  actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessage));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_EQ(testMessage, actualMessage);
}

TEST(Logger, ANSI)
{
  auto stringSink = std::make_shared<FakeStringLogSink>();
  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  logger.AddListener(stringSink);

  const char* szTestMessage = "Hello World\n";
  std::string testMessage = szTestMessage;

  MKL_LOGI(&logger, szTestMessage);
  std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(szTestMessage));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_STREQ(szTestMessage, actualMessage.c_str());

  stringSink->Buffer.clear();

  MKL_LOGI(&logger, testMessage);
  actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessage));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_EQ(testMessage, actualMessage);
}

TEST(Logger, UCS2)
{
  auto stringSink = std::make_shared<FakeStringLogSink>();
  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  logger.AddListener(stringSink);

  const wchar_t* szTestMessage = L"Hello World\n";
  std::wstring testMessage = szTestMessage;
  std::string testMessageUtf8 = Ucs2ToUtf8(szTestMessage);

  MKL_LOGI(&logger, szTestMessage);
  std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessageUtf8));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_EQ(testMessageUtf8, actualMessage);

  stringSink->Buffer.clear();

  MKL_LOGI(&logger, testMessage);
  actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessageUtf8));
  actualMessage.pop_back(); // remove the \n that was added by the logger
  EXPECT_EQ(testMessageUtf8, actualMessage);
}

TEST(Logger, CombineStringTypes)
{
  std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
  EnsureCleanOutputDirectory(testOuputDirectoryPath);
  auto unbufferedLogPath = testOuputDirectoryPath / "unbuffered.log";
  auto unbufferedLogFile = std::make_shared<LogUnbufferedFileSink>();
  EXPECT_TRUE(unbufferedLogFile->Create(unbufferedLogPath));

  auto bufferedLogPath = testOuputDirectoryPath / "buffered.log";
  auto bufferedLogFile = std::make_shared<LogFileSink>();
  EXPECT_TRUE(bufferedLogFile->Create(bufferedLogPath));

  auto stringSink = std::make_shared<FakeStringLogSink>();

  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  logger.SetFormatter(nullptr);
  logger.AddListener(stringSink);
  logger.AddListener(bufferedLogFile);
  logger.AddListener(unbufferedLogFile);
  logger.AddListener(std::make_shared<LogDebugOutputSink>());

  MKL_LOGI(&logger, "\n\tHello world" << L"\n\tHello world" << u8"\n\t你好世界" << L"\n\t你好世界" << L"\n\t\x4f60\x597d\x4e16\x754c");

  auto helloWorldChinese = u8"\n\t你好世界";

  // string buffer
  EXPECT_EQ(0u, stringSink->Buffer.find("\n\tHello world"));
  EXPECT_EQ(13u, stringSink->Buffer.find("\n\tHello world", 1));
  EXPECT_EQ(26u, stringSink->Buffer.find(helloWorldChinese));
  EXPECT_EQ(40u, stringSink->Buffer.find(helloWorldChinese, 27));
  EXPECT_EQ(54u, stringSink->Buffer.find(helloWorldChinese, 41));

  // buffered log file
  std::string actualBufferedData = ReadLogFileAsBinary(bufferedLogPath);
  EXPECT_TRUE(actualBufferedData.empty());
  // flush/close the buffered log file
  bufferedLogFile->Close();
  actualBufferedData = ReadLogFileAsBinary(bufferedLogPath);
  actualBufferedData.erase(0, 3); // remove the UTF8 BOM
  EXPECT_EQ(0u, actualBufferedData.find("\n\tHello world"));
  EXPECT_EQ(13u, actualBufferedData.find("\n\tHello world", 1));
  EXPECT_EQ(26u, actualBufferedData.find(helloWorldChinese));
  EXPECT_EQ(40u, actualBufferedData.find(helloWorldChinese, 27));
  EXPECT_EQ(54u, actualBufferedData.find(helloWorldChinese, 41));

  std::string actualUnbufferedData = ReadLogFileAsBinary(unbufferedLogPath);
  actualUnbufferedData.erase(0, 3); // remove the UTF8 BOM
  EXPECT_EQ(0u, actualUnbufferedData.find("\n\tHello world"));
  EXPECT_EQ(13u, actualUnbufferedData.find("\n\tHello world", 1));
  EXPECT_EQ(26u, actualUnbufferedData.find(helloWorldChinese));
  EXPECT_EQ(40u, actualUnbufferedData.find(helloWorldChinese, 27));
  EXPECT_EQ(54u, actualUnbufferedData.find(helloWorldChinese, 41));
}

TEST(Logger, MultipleThreadsWithDebugOutputAndQueuedLogFile)
{
  std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
  EnsureCleanOutputDirectory(testOuputDirectoryPath);
  std::filesystem::path logFilePath = testOuputDirectoryPath / L"test.log";

  // Make something that looks like a realistic logging configuration (debug and file output)
  std::shared_ptr<LogFileSink> logFile = std::make_shared<LogFileSink>();
  EXPECT_TRUE(logFile->Create(logFilePath));
  std::shared_ptr<LogQueue> bufferedFile = std::make_shared<LogQueue>(logFile);

  std::shared_ptr<LogDebugOutputSink> debugOutput = std::make_shared<LogDebugOutputSink>();

  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::Info); // set before listeners to avoid irrelevant warnings in the log
  logger.AddListener(debugOutput);
  logger.AddListener(bufferedFile);

  std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

  constexpr int threadCount = 4;
  std::vector<std::thread> logThreads;
  for (int i = 0; i < threadCount; i++)
  {
    std::thread t([&logger]
      {
        for (int bottlesOfBeer = 99; bottlesOfBeer > 0; bottlesOfBeer--)
        {
          MKL_LOGW(&logger, "\n" << bottlesOfBeer << " bottles of beer on the wall, " << bottlesOfBeer << " bottles of beer.\n"
            << "Take one down and pass it around, " << bottlesOfBeer - 1 << " bottles of beer on the wall.");
        }
      }
    );
    logThreads.push_back(std::move(t));
  }
  size_t intermediateQueueLength = bufferedFile->GetMessageQueueSize();
  std::chrono::system_clock::time_point intermediateTime = std::chrono::system_clock::now();

  // wait until all the producer threads are done
  for (std::thread& t : logThreads)
  {
    if (t.joinable())
    {
      t.join();
    }
  }
  size_t nProducedQueueLength = bufferedFile->GetMessageQueueSize();
  std::chrono::system_clock::time_point producedTime = std::chrono::system_clock::now();

  bufferedFile->Drain();
  std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();

  // log some statistics
  MKL_LOGW(&logger, ""
    << "\n\tIt took the main thread " << std::chrono::duration_cast<std::chrono::microseconds>(intermediateTime - startTime).count() << " us to set up the production"
    << "\n\t and there were " << intermediateQueueLength << " items in the queue at that time"
    << "\n\tProduction took " << std::chrono::duration_cast<std::chrono::milliseconds>(producedTime - startTime).count() << " ms in total"
    << "\n\tWhen log production stopped, there were " << nProducedQueueLength << " items in the queue"
    << "\n\tDraining the queue took " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - producedTime).count() << " ms"
    << "\n\tTotal duration=" << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms"
  );

  logger.RemoveListener(bufferedFile);
  bufferedFile->Drain(); // drain again to get the statistics in the file before the end of the test
  logFile->Close();
  bufferedFile.reset();

  // Check the contents of the log file
  std::string logFileText = ReadLogFileAsBinary(logFilePath);
  EXPECT_EQ(threadCount, CountOccurrence(logFileText, "Take one down and pass it around, 0 bottles of beer on the wall."));

  // do the same test using a FILE structure
  std::FILE* file = _wfsopen(logFilePath.c_str(), L"rt", _SH_DENYWR);
  ASSERT_NE(nullptr, file);
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  std::string newLogFileText(fileSize, '\0');
  size_t nReadItemCount = fread_s(newLogFileText.data(), sizeof(char) * newLogFileText.size(), sizeof(char), newLogFileText.size(), file);
  fclose(file);
  ASSERT_EQ(nReadItemCount, newLogFileText.size()); // newline translation in effect - number of characters in buffer should be less than the file size
  newLogFileText.resize(nReadItemCount);
  EXPECT_EQ(logFileText, newLogFileText);
}

TEST(Logger, ChainOfLoggers)
{
  auto leafLogger = std::make_shared<Logger>();
  leafLogger->SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  auto mockLeafSink = std::make_shared<MockLogSink>();
  leafLogger->AddListener(mockLeafSink);

  auto mockRootSink = std::make_shared<MockLogSink>();
  Logger rootLogger;
  rootLogger.SetMinimumLogLevel(ELogLevel::All); // set before listeners to avoid irrelevant warnings in the log
  rootLogger.AddListener(leafLogger);
  rootLogger.AddListener(mockRootSink);
  rootLogger.AddListener(std::make_shared<LogDebugOutputSink>());

  // set expectations
  EXPECT_CALL(*mockLeafSink, OutputRecord).Times(2);
  EXPECT_CALL(*mockRootSink, OutputRecord).Times(1);

  // run the tests
  MKL_LOGV(&rootLogger, "test");
  MKL_LOGV(leafLogger.get(), "test");
}

TEST(Logger, ChainOfLoggers2)
{
  // Setup: a LogCentral (for debugging), an ErrorLog that listens to LogCentral and only forwards Errors, and a VerboseLog that is too verbose to write its output in LogCentral (LogDebugOutputSink). Each logger has 1 sink - for testing.
  auto myLogCentral = std::make_shared<Logger>();
  myLogCentral->SetFormatter(nullptr);
  myLogCentral->SetMinimumLogLevel(ELogLevel::Debug); // set before listeners to avoid irrelevant warnings in the log

  auto myErrorLog = std::make_shared<Logger>();
  myErrorLog->SetFormatter(nullptr);
  myErrorLog->SetMinimumLogLevel(ELogLevel::Error);

  myLogCentral->AddListener(std::make_shared<LogDebugOutputSink>());
  auto mockCentralSink = std::make_shared<MockLogSink>();
  myLogCentral->AddListener(mockCentralSink);

  auto mockErrorSink = std::make_shared<MockLogSink>();
  myErrorLog->AddListener(mockErrorSink);
  myLogCentral->AddListener(myErrorLog);

  auto myVerboseLog = std::make_shared<Logger>();
  myVerboseLog->SetMinimumLogLevel(ELogLevel::All);
  auto mockVerboseSink = std::make_shared<MockLogSink>();
  myVerboseLog->AddListener(mockVerboseSink);
  myLogCentral->AddListener(myVerboseLog);

  // set expectations
  EXPECT_CALL(*mockCentralSink, OutputRecord).Times(4);
  EXPECT_CALL(*mockErrorSink, OutputRecord).Times(1);
  EXPECT_CALL(*mockVerboseSink, OutputRecord).Times(5);

  MKL_LOGV(myLogCentral.get(), "test"); // filtered out by LogCentral
  MKL_LOGD(myLogCentral.get(), "test"); // sinks in [central,verbose]
  MKL_LOGI(myLogCentral.get(), "test"); // sinks in [central,verbose]
  MKL_LOGW(myLogCentral.get(), "test"); // sinks in [central,verbose]
  MKL_LOGE(myLogCentral.get(), "test"); // sinks in [central,verbose,error]

  MKL_LOGV(myVerboseLog.get(), "test"); // sinks in [verbose]
}
