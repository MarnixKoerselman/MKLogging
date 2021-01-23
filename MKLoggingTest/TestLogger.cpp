#include <Logger.h>
#include <LogUtf8FileSink.h>

#include <string>
#include <filesystem>

#include <gtest/gtest.h>
#include "TestUtils.h"
#include "FakeStringLogSink.h"

#include <atlfile.h>
#include <StringUtils.h>
#include <LogQueue.h>
#include <LogDebugOutputSink.h>


using DataBuffer = std::vector<std::byte>; // requires C++17

int CountOccurrence(const std::wstring& string, const std::wstring& subString)
{
    size_t nOffset = 0, n = 0;
    int iCount = 0;
    while (n = string.find(subString, nOffset), n != std::wstring::npos)
    {
        iCount += 1;
        nOffset = n + subString.length();
    }
    return iCount;
}

// IntegrationTest: use global instance g_LogCollector

TEST(IntegrationTest, DefaultBehaviour)
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
    LOGD(L"hello debug");
    LOGD("hello debug in utf-8");
    LOGD(L"你好"); // NB: this source file was saved with UTF-8 encoding to make this test possible
    LOGI("hello information in utf-8");
    LOGW(L"hello warning");
    LOGE(L"hello error");
    LOGD(true);
    LOGI(false);

    // Test that there are no log files in the output directory
    DirectoryEntries finalDirectoryEntries = GetDirectoryContents(testOuputDirectoryPath);
    EXPECT_EQ(initialDirectoryEntries, finalDirectoryEntries);
}

TEST(IntegrationTest, LogSinkInterface)
{
    std::shared_ptr<FakeStringLogSink> pStringSink = std::make_shared<FakeStringLogSink>();
    CLogCentral logger({ pStringSink });

    MKL_LOGE(logger, L"test");

    std::wstring sBuffer = pStringSink->Buffer;
    EXPECT_NE(std::wstring::npos, sBuffer.find(L"test"));
    // The method name ('log source') is also in the log
    EXPECT_NE(std::wstring::npos, sBuffer.find(L"IntegrationTest")); // see TEST(,) statement
    EXPECT_NE(std::wstring::npos, sBuffer.find(L"LogSinkInterface")); // see TEST(,) statement
}

TEST(UnitTest, LogLevels)
{
    CLogCentral localLogger({});

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


TEST(IntegrationTest, LogFileUtf8)
{
    std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
    EnsureCleanOutputDirectory(testOuputDirectoryPath);

    std::filesystem::path logFilePath = testOuputDirectoryPath / L"test.log";
    std::wstring sTestMessage = L"Hello World\n\x4f60\x597d\x4e16\x754c"; // Hello World in Traditional Chinese, and in English
    {
        CLogUtf8FileSink logFile;
        EXPECT_TRUE(logFile.Create(testOuputDirectoryPath, logFilePath.filename()));
        logFile.OutputString(sTestMessage);
    }
    // the log file is now flushed and closed and ready for evaluation

    CAtlFile logFile;
    ASSERT_HRESULT_SUCCEEDED(logFile.Create(logFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING));
    unsigned long long nFileSize = 0;
    ASSERT_HRESULT_SUCCEEDED(logFile.GetSize(nFileSize));
    DataBuffer buffer(static_cast<size_t>(nFileSize));
    ASSERT_HRESULT_SUCCEEDED(logFile.Read(buffer.data(), static_cast<DWORD>(buffer.size())));

    // Convert the UTF-8 file content back to UTF-16 / UCS-2 for comparison with the original
    std::wstring sActualOutput = Utf8ToUtf16(std::string(reinterpret_cast<const char*>(buffer.data()), buffer.size()));

    std::wstring sExpectedOutput = L'\xFEFF' + sTestMessage; // prefix with BOM for UTF-8 
    StringReplaceAll(sExpectedOutput, L"\n", L"\r\n"); // ... windows uses \r\n instead of \n ...
    EXPECT_EQ(sExpectedOutput, sActualOutput);
}

TEST(IntegrationTest, MultiThreadedLogQueue)
{
    std::shared_ptr<FakeStringLogSink> pStringLog = std::make_shared<FakeStringLogSink>();
    std::shared_ptr<CLogQueue> pBufferedLog = std::make_shared<CLogQueue>(pStringLog);
    CLogCentral logger({ pBufferedLog, std::make_shared<CLogDebugOutputSink>() });
    //CLogCentral logger({ pBufferedLog });
    logger.SetMinimumLogLevel(ELogLevel::Debug);

    const int numberOfLogsPerThread = 5;

    // create a number of of log producers
    std::list<std::thread> producers;
    producers.push_back(std::thread([&logger]
        {
            //DebugBreak();
            MKL_LOGV(logger);
        }));
    producers.push_back(std::thread([&logger, numberOfLogsPerThread]
        {
            //DebugBreak();
            for (int iLog = 0; iLog < numberOfLogsPerThread; iLog++)
            {
                MKL_LOGD(logger, "This is test " << iLog << " in thread 0x" << std::hex << std::this_thread::get_id());
            }
        }));
    producers.push_back(std::thread([&logger, numberOfLogsPerThread]
        {
            //DebugBreak();
            for (int iLog = 0; iLog < numberOfLogsPerThread; iLog++)
            {
                MKL_LOGI(logger, "This is test " << iLog << " in thread 0x" << std::hex << std::this_thread::get_id());
            }
        }));
    producers.push_back(std::thread([&logger, numberOfLogsPerThread]
        {
            //DebugBreak();
            for (int iLog = 0; iLog < numberOfLogsPerThread; iLog++)
            {
                MKL_LOGW(logger, "This is test " << iLog << " in thread 0x" << std::hex << std::this_thread::get_id());
            }
        }));
    producers.push_back(std::thread([&logger, numberOfLogsPerThread]
        {
            //DebugBreak();
            for (int iLog = 0; iLog < numberOfLogsPerThread; iLog++)
            {
                MKL_LOGE(logger, "This is test " << iLog << " in thread 0x" << std::hex << std::this_thread::get_id());
            }
        }));

    // wait for completion of all threads
    for (auto& t : producers)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    // evaluate the output
    //OutputDebugString(pStringLog->Buffer.c_str()); // NB this does not print the whole string!

    // wait until all items in the queue have been handled
    // this test is actually quite slow; the consumer thread is usually finished by the time the main thread gets here... things are different when there are more and slower log sinks attached to the logger
    pBufferedLog->Drain();

    // example:
    // 2019-08-21 14:08:57 (0x0000438C) DEBUG     IntegrationTest_MultiThreadedLogQueue_Test::TestBody::<lambda_8ec409d8ff2bd8ffef1972bb98326902>::operator (): This is test 0 in thread 0x438c
    for (int i = 0; i < numberOfLogsPerThread; i++)
    {
        std::wstring sExpected = FormatString(L"This is test %d in thread 0x", i);
        // this string is expected for LOGD/I/W/E logs, but not for LOGV
        EXPECT_EQ(4, CountOccurrence(pStringLog->Buffer, sExpected));
    }

    EXPECT_EQ(0, CountOccurrence(pStringLog->Buffer, ELogLevel_ToString(ELogLevel::Verbose)));
    EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(pStringLog->Buffer, ELogLevel_ToString(ELogLevel::Debug)));
    EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(pStringLog->Buffer, ELogLevel_ToString(ELogLevel::Info)));
    EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(pStringLog->Buffer, ELogLevel_ToString(ELogLevel::Warning)));
    EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(pStringLog->Buffer, ELogLevel_ToString(ELogLevel::Error)));
}

TEST(IntegrationTest, ImplementationMultipleThreads)
{
    std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
    EnsureCleanOutputDirectory(testOuputDirectoryPath);
    std::filesystem::path logFilePath = testOuputDirectoryPath / L"test.log";

    // Make something that looks like a realistic logging configuration (debug and file output)
    std::shared_ptr<CLogUtf8FileSink> pLogFile = std::make_shared<CLogUtf8FileSink>();
    EXPECT_TRUE(pLogFile->Create(testOuputDirectoryPath, logFilePath.filename()));
    std::shared_ptr<CLogQueue> pBufferedFile = std::make_shared<CLogQueue>(pLogFile);

    std::shared_ptr<CLogDebugOutputSink> pDebug = std::make_shared<CLogDebugOutputSink>();

    CLogCentral logger({ pDebug, pBufferedFile }); // this is the 'local' Code Under Test replacement of the global g_LogCentral
    logger.SetMinimumLogLevel(ELogLevel::Info);

    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

    constexpr int iThreadCount = 4;
    std::vector<std::thread> logThreads;
    for (int i = 0; i < iThreadCount; i++)
    {
        std::thread t([&logger]
            {
                for (int iBottlesOfBeer = 99; iBottlesOfBeer > 0; iBottlesOfBeer--)
                {
                    // NB: by default only warnings and errors are logged in a Release build
                    MKL_LOGW(logger, L"\n" << iBottlesOfBeer << L" bottles of beer on the wall, " << iBottlesOfBeer << L" bottles of beer.\n"
                        << L"Take one down and pass it around, " << iBottlesOfBeer - 1 << L" bottles of beer on the wall.");
                }
            }
        );
        logThreads.push_back(std::move(t));
    }
    size_t intermediateQueueLength = pBufferedFile->GetMessageQueueSize();
    std::chrono::system_clock::time_point intermediateTime = std::chrono::system_clock::now();

    // wait until all the producer threads are done
    for (std::thread& t : logThreads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    size_t nProducedQueueLength = pBufferedFile->GetMessageQueueSize();
    std::chrono::system_clock::time_point producedTime = std::chrono::system_clock::now();

    pBufferedFile->Drain();
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
    // TODO: why does it take so long to drain the queue when the system is at rest??

    logger.RemoveListener(pBufferedFile);
    pBufferedFile->Drain(); // drain again to get the statistics in the file before the end of the test
    pLogFile->Close();
    pBufferedFile.reset();

    // Check the contents of the log file
    CAtlFile logFile;
    EXPECT_HRESULT_SUCCEEDED(logFile.Create(logFilePath.c_str(), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, OPEN_EXISTING));
    unsigned long long logFileSize = 0;
    EXPECT_HRESULT_SUCCEEDED(logFile.GetSize(logFileSize));
    DataBuffer logFileData(static_cast<unsigned int>(logFileSize));
    EXPECT_HRESULT_SUCCEEDED(logFile.Read(logFileData.data(), static_cast<DWORD>(logFileData.size())));
    logFile.Close();
    // first 3 bytes are the UTF8 BOM: 0xEF,0xBB,0xBF
    EXPECT_EQ(0xEF, std::to_integer<uint8_t>(logFileData[0]));
    EXPECT_EQ(0xBB, std::to_integer<uint8_t>(logFileData[1]));
    EXPECT_EQ(0xBF, std::to_integer<uint8_t>(logFileData[2]));
    std::string sUtf8(reinterpret_cast<const char*>(logFileData.data() + 3), logFileData.size() - 3);
    std::wstring sLogFileText(sUtf8.begin(), sUtf8.end()); // naive conversion from UTF8 to UTF16/UCS2, barely good enough for this test
    EXPECT_EQ(iThreadCount, CountOccurrence(sLogFileText, L"Take one down and pass it around, 0 bottles of beer on the wall."));

    // do the same test using a FILE structure
    std::FILE* pFile = _wfsopen(logFilePath.c_str(), L"rt, ccs=UTF-8", _SH_DENYWR);
    ASSERT_NE(nullptr, pFile);
    // "When a file is opened in Unicode mode, input functions translate the data that's read from the file into UTF-16 data stored as type wchar_t."
    std::vector<wchar_t> fileText;
    std::vector<wchar_t> readBuffer(10240000);
    while (!feof(pFile))
    {
        size_t nReadItemCount = fread_s(readBuffer.data(), sizeof(wchar_t) * readBuffer.size(), sizeof(wchar_t), readBuffer.size(), pFile);
        fileText.insert(fileText.end(), readBuffer.cbegin(), readBuffer.cbegin() + nReadItemCount);
    }
    fclose(pFile);
    std::wstring sNewLogFileText(fileText.data(), fileText.size());

    // fileText is not exactly the same as sLogFileText, because of the \n to \r\n conversion when writing, which is reversed when reading the file
    // in the POSIX way (as opposed to binary reading in the previous test).
    StringReplaceAll(sLogFileText, L"\r\n", L"\n"); // replace the \r\n line endings that are added in Windows when using fwrite on text files
    EXPECT_EQ(sLogFileText, sNewLogFileText); // now they should be the same
}
