#include <Logger.h>
#include <LogFileSink.h>
#include <LogQueue.h>
#include <LogDebugOutputSink.h>
#include <LogUnbufferedFileSink.h>

#include <string>
#include <filesystem>

#include <gtest/gtest.h>
#include "TestUtils.h"
#include "FakeStringLogSink.h"
#include "StringUtils.h"

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

    LOGV();
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
    std::shared_ptr<FakeStringLogSink> stringSink = std::make_shared<FakeStringLogSink>();
    CLogCentral logger({ stringSink });

    MKL_LOGE(logger, "test");

    std::string buffer = stringSink->Buffer;
    EXPECT_NE(std::string::npos, buffer.find("test"));
    // The method name ('log source') is also in the log
    EXPECT_NE(std::string::npos, buffer.find("Logger")); // see function header
    EXPECT_NE(std::string::npos, buffer.find("LogSinkInterface")); // see function header
}

TEST(Logger, LogLevelFilter)
{
    CLogCentral localLogger({ std::make_shared<FakeStringLogSink>() });

    for (ELogLevel eSetLogLevel = ELogLevel::Min; eSetLogLevel <= ELogLevel::Max; ++eSetLogLevel)     {
        localLogger.SetMinimumLogLevel(eSetLogLevel);

        for (ELogLevel eExpectLogLevel = ELogLevel::Min; eExpectLogLevel <= ELogLevel::Max; ++eExpectLogLevel)         {
            if (eExpectLogLevel < eSetLogLevel)             {
                EXPECT_FALSE(localLogger.IsLogged(eExpectLogLevel));
            }
            else             {
                EXPECT_TRUE(localLogger.IsLogged(eExpectLogLevel));
            }
        }
    }
}

TEST(Logger, LogLevelFilterWithNoLogSink)
{
    CLogCentral localLogger({});

    for (ELogLevel eSetLogLevel = ELogLevel::Min; eSetLogLevel <= ELogLevel::Max; ++eSetLogLevel)     {
        localLogger.SetMinimumLogLevel(eSetLogLevel);

        for (ELogLevel eExpectLogLevel = ELogLevel::Min; eExpectLogLevel <= ELogLevel::Max; ++eExpectLogLevel)         {
            EXPECT_FALSE(localLogger.IsLogged(eExpectLogLevel));
        }
    }
}

TEST(Logger, Utf8)
{
    auto stringSink = std::make_shared<FakeStringLogSink>();
    CLogCentral logger({ stringSink });
    logger.SetMinimumLogLevel(ELogLevel::All);

    const char* szTestMessage = u8"Hello World\n你好世界";
    std::string testMessage = szTestMessage;

    MKL_LOGI(logger, szTestMessage);
    std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(szTestMessage));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_STREQ(szTestMessage, actualMessage.c_str());

    stringSink->Buffer.clear();

    MKL_LOGI(logger, testMessage);
    actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessage));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_EQ(testMessage, actualMessage);
}

TEST(Logger, ANSI)
{
    auto stringSink = std::make_shared<FakeStringLogSink>();
    CLogCentral logger({ stringSink });
    logger.SetMinimumLogLevel(ELogLevel::All);

    const char* szTestMessage = "Hello World\n";
    std::string testMessage = szTestMessage;

    MKL_LOGI(logger, szTestMessage);
    std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(szTestMessage));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_STREQ(szTestMessage, actualMessage.c_str());

    stringSink->Buffer.clear();

    MKL_LOGI(logger, testMessage);
    actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessage));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_EQ(testMessage, actualMessage);
}

TEST(Logger, UCS2)
{
    auto stringSink = std::make_shared<FakeStringLogSink>();
    CLogCentral logger({ stringSink });
    logger.SetMinimumLogLevel(ELogLevel::All);

    const wchar_t* szTestMessage = L"Hello World\n";
    std::wstring testMessage = szTestMessage;
    std::string testMessageUtf8 = Ucs2ToUtf8(szTestMessage);

    MKL_LOGI(logger, szTestMessage);
    std::string actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessageUtf8));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_EQ(testMessageUtf8, actualMessage);

    stringSink->Buffer.clear();

    MKL_LOGI(logger, testMessage);
    actualMessage = stringSink->Buffer.substr(stringSink->Buffer.rfind(testMessageUtf8));
    actualMessage.pop_back(); // remove the \n that was added by the logger
    EXPECT_EQ(testMessageUtf8, actualMessage);
}

TEST(Logger, CombineStringTypes)
{
    std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
    EnsureCleanOutputDirectory(testOuputDirectoryPath);
    auto unbufferedLogPath = testOuputDirectoryPath / "unbuffered.log";
    auto bufferedLogPath = testOuputDirectoryPath / "buffered.log";

    auto bufferedLogFile = std::make_shared<CLogFileSink>();
    EXPECT_TRUE(bufferedLogFile->Create(bufferedLogPath));

    auto unbufferedLogFile = std::make_shared<CLogUnbufferedFileSink>();
    EXPECT_TRUE(unbufferedLogFile->Create(unbufferedLogPath));

    auto stringSink = std::make_shared<FakeStringLogSink>();

    CLogCentral logger({ stringSink, bufferedLogFile, unbufferedLogFile, std::make_shared<CLogDebugOutputSink>() });
    logger.SetMinimumLogLevel(ELogLevel::All);

    MKL_LOGI(logger, "\n\tHello world" << L"\n\tHello world" << u8"\n\t你好世界" << L"\n\t你好世界" << L"\n\t\x4f60\x597d\x4e16\x754c");

    auto helloWorldChinese = u8"\n\t你好世界";

    // string buffer
    EXPECT_EQ(89u, stringSink->Buffer.find("\n\tHello world"));
    EXPECT_EQ(102u, stringSink->Buffer.find("\n\tHello world", 90));
    EXPECT_EQ(115u, stringSink->Buffer.find(helloWorldChinese));
    EXPECT_EQ(129u, stringSink->Buffer.find(helloWorldChinese, 116));
    EXPECT_EQ(143u, stringSink->Buffer.find(helloWorldChinese, 130));

    // buffered log file
    std::string actualBufferedData = ReadLogFileAsBinary(bufferedLogPath);
    EXPECT_TRUE(actualBufferedData.empty());
    // flush/close the buffered log file
    bufferedLogFile->Close();
    actualBufferedData = ReadLogFileAsBinary(bufferedLogPath);
    EXPECT_EQ(92u, actualBufferedData.find("\n\tHello world"));
    EXPECT_EQ(105u, actualBufferedData.find("\n\tHello world", 93));
    EXPECT_EQ(118u, actualBufferedData.find(helloWorldChinese));
    EXPECT_EQ(132u, actualBufferedData.find(helloWorldChinese, 119));
    EXPECT_EQ(146u, actualBufferedData.find(helloWorldChinese, 133));

    std::string actualUnbufferedData = ReadLogFileAsBinary(unbufferedLogPath);
    EXPECT_EQ(92u, actualUnbufferedData.find("\n\tHello world"));
    EXPECT_EQ(105u, actualUnbufferedData.find("\n\tHello world", 93));
    EXPECT_EQ(118u, actualUnbufferedData.find(helloWorldChinese));
    EXPECT_EQ(132u, actualUnbufferedData.find(helloWorldChinese, 119));
    EXPECT_EQ(146u, actualUnbufferedData.find(helloWorldChinese, 133));
}

TEST(Logger, MultipleThreadsWithDebugOutputAndQueuedLogFile)
{
    std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
    EnsureCleanOutputDirectory(testOuputDirectoryPath);
    std::filesystem::path logFilePath = testOuputDirectoryPath / L"test.log";

    // Make something that looks like a realistic logging configuration (debug and file output)
    std::shared_ptr<CLogFileSink> logFile = std::make_shared<CLogFileSink>();
    EXPECT_TRUE(logFile->Create(logFilePath));
    std::shared_ptr<CLogQueue> bufferedFile = std::make_shared<CLogQueue>(logFile);

    std::shared_ptr<CLogDebugOutputSink> debugOutput = std::make_shared<CLogDebugOutputSink>();

    CLogCentral logger({ debugOutput, bufferedFile }); // this is the 'local' Code Under Test replacement of the global g_LogCentral
    logger.SetMinimumLogLevel(ELogLevel::Info);

    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

    constexpr int threadCount = 4;
    std::vector<std::thread> logThreads;
    for (int i = 0; i < threadCount; i++)
    {
        std::thread t([&logger]
            {
                for (int bottlesOfBeer = 99; bottlesOfBeer > 0; bottlesOfBeer--)
                {
                    // NB: by default only warnings and errors are logged in a Release build
                    MKL_LOGW(logger, "\n" << bottlesOfBeer << " bottles of beer on the wall, " << bottlesOfBeer << " bottles of beer.\n"
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
    MKL_LOGW(logger, ""
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
