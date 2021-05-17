#include <Logger.h>
#include <LogFileSink.h>

#include <string>
#include <filesystem>

#include <gtest/gtest.h>
#include "TestUtils.h"
#include "FakeStringLogSink.h"

#include "StringUtils.h"
#include <LogQueue.h>
#include <LogDebugOutputSink.h>

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

TEST(LogFileSink, Basic)
{
    std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
    EnsureCleanOutputDirectory(testOuputDirectoryPath);

    std::filesystem::path logFilePath = testOuputDirectoryPath / "test.log";
    std::string testMessage = "Hello World\n";
    {
        CLogFileSink logFile;
        ASSERT_TRUE(logFile.Create(logFilePath));
        logFile.OutputString(testMessage);
    }
    // the log file is now flushed and closed and ready for evaluation

    std::string actualOutput = ReadLogFileAsBinary(logFilePath);

    std::string expectedOutput = testMessage;
    StringReplaceAll(expectedOutput, "\n", "\r\n"); // ... windows uses \r\n instead of \n ...
    EXPECT_EQ(expectedOutput, actualOutput);
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
    ASSERT_LT(nReadItemCount, newLogFileText.size()); // newline translation in effect - number of characters in buffer should be less than the file size
    newLogFileText.resize(nReadItemCount);

    // newLogFileText is not exactly the same as logFileText, because of the \n to \r\n conversion when writing, which is reversed when reading the file
    // in the POSIX way (as opposed to binary reading in the previous test).
    EXPECT_NE(logFileText, newLogFileText); // they should have different line endings
    StringReplaceAll(logFileText, "\r\n", "\n"); // replace the \r\n line endings that are added in Windows when using fwrite on text files
    EXPECT_EQ(logFileText, newLogFileText); // now they should be the same
}
