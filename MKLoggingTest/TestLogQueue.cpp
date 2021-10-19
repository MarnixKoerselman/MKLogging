// what we're testing:
#include "LogQueue.h"

// what we need for for testing
#include "FakeFileLogSink.h"
#include "FakeStringLogSink.h"
#include "MockLogSink.h"
#include <LogDebugOutputSink.h>
#include "StringUtils.h"
#include "TestUtils.h"

TEST(LogQueue, Basic)
{
	// Initialise the test environment
	auto mockSink = std::make_shared<MockLogSink>();
	std::shared_ptr<CLogQueue> bufferedSink = std::make_shared<CLogQueue>(mockSink);

	// Set expectations
	std::string testString = "This is a test";
	LogRecord testRecord(bufferedSink.get(), ELogLevel::All, __FUNCTION__, __FILE__, __LINE__);
	testRecord.Get() << testString;

	EXPECT_CALL(*mockSink, OutputRecord(testRecord))
		.RetiresOnSaturation();

	// Run the test
	bufferedSink->OutputRecord(testRecord);
	// Waiting for things to happen in a multi-threaded environment is always a little tricky, particularly because adding timers (sleep) to the test would make the tests unnecessarily slow
	while (bufferedSink->GetMessageQueueSize() > 0)
	{
		std::this_thread::yield();
	}
}

TEST(LogQueue, MultiThreaded)
{
	std::shared_ptr<FakeStringLogSink> stringLog = std::make_shared<FakeStringLogSink>();
	std::shared_ptr<CLogQueue> bufferedLog = std::make_shared<CLogQueue>(stringLog);
	CLogCentral logger;
	logger.AddListener(bufferedLog);
	logger.AddListener(std::make_shared<CLogDebugOutputSink>());
	logger.SetMinimumLogLevel(ELogLevel::Debug);

	const int numberOfLogsPerThread = 5;

	// create a number of of log producers
	std::list<std::thread> producers;
	producers.push_back(std::thread([&logger]
									{
										MKL_LOGV(logger);
									}));
	producers.push_back(std::thread([&logger, numberOfLogsPerThread]
									{
										for (int i = 0; i < numberOfLogsPerThread; i++)
										{
											MKL_LOGD(logger, "This is test " << i << " in thread 0x" << std::hex << std::this_thread::get_id());
										}
									}));
	producers.push_back(std::thread([&logger, numberOfLogsPerThread]
									{
										for (int i = 0; i < numberOfLogsPerThread; i++)
										{
											MKL_LOGI(logger, "This is test " << i << " in thread 0x" << std::hex << std::this_thread::get_id());
										}
									}));
	producers.push_back(std::thread([&logger, numberOfLogsPerThread]
									{
										for (int i = 0; i < numberOfLogsPerThread; i++)
										{
											MKL_LOGW(logger, "This is test " << i << " in thread 0x" << std::hex << std::this_thread::get_id());
										}
									}));
	producers.push_back(std::thread([&logger, numberOfLogsPerThread]
									{
										for (int i = 0; i < numberOfLogsPerThread; i++)
										{
											MKL_LOGE(logger, "This is test " << i << " in thread 0x" << std::hex << std::this_thread::get_id());
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
	//OutputDebugString(stringLog->Buffer.c_str()); // NB this does not print the whole string!

	// wait until all items in the queue have been handled
	// this test is actually quite slow; the consumer thread is usually finished by the time the main thread gets here... things are different when there are more and slower log sinks attached to the logger
	bufferedLog->Drain();

	// example:
	// 2019-08-21 14:08:57 (0x0000438C) DEBUG     IntegrationTest_MultiThreadedLogQueue_Test::TestBody::<lambda_8ec409d8ff2bd8ffef1972bb98326902>::operator (): This is test 0 in thread 0x438c
	for (int i = 0; i < numberOfLogsPerThread; i++)
	{
		std::string expected = FormatString("This is test %d in thread 0x", i);
		// this string is expected for LOGD/I/W/E logs, but not for LOGV
		EXPECT_EQ(4, CountOccurrence(stringLog->Buffer, expected));
	}

	EXPECT_EQ(0, CountOccurrence(stringLog->Buffer, ELogLevel_ToString(ELogLevel::Verbose)));
	EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(stringLog->Buffer, ELogLevel_ToString(ELogLevel::Debug)));
	EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(stringLog->Buffer, ELogLevel_ToString(ELogLevel::Info)));
	EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(stringLog->Buffer, ELogLevel_ToString(ELogLevel::Warning)));
	EXPECT_EQ(numberOfLogsPerThread, CountOccurrence(stringLog->Buffer, ELogLevel_ToString(ELogLevel::Error)));
}
