// what we're testing:
#include "MKLogging/LogQueue.h"

// what we need for for testing
#include "FakeFileLogSink.h"
#include "FakeStringLogSink.h"
#include "MockLogSink.h"
#ifdef WIN32
#include "MKLogging/Windows/LogDebugOutputSink.h"
#endif
#include "StringUtils.h"
#include "TestUtils.h"
#include <list>

using namespace MKLogging;

TEST(LogQueue, Basic)
{
  // Initialise the test environment
  auto mockSink = std::make_shared<MockLogSink>();
  std::shared_ptr<LogQueue> bufferedSink = std::make_shared<LogQueue>(mockSink);

  // Set expectations
  std::string testString = "This is a test";
  LogRecord testRecord(ELogLevel::All, __FUNCTION__, __FILE__, __LINE__);
  testRecord.SetMessage(testString);

  EXPECT_CALL(*mockSink, OutputRecord(testRecord)).Times(1);

  // Run the test
  bufferedSink->OutputRecord(testRecord);
  while (bufferedSink->GetMessageQueueSize() > 0)
  {
    std::this_thread::yield();
  }
}

TEST(LogQueue, MultiThreaded)
{
  std::shared_ptr<FakeStringLogSink> stringLog = std::make_shared<FakeStringLogSink>();
  std::shared_ptr<LogQueue> bufferedLog = std::make_shared<LogQueue>(stringLog);
  Logger logger;
  logger.SetMinimumLogLevel(ELogLevel::Debug); // set before listeners to avoid irrelevant warnings in the log
  logger.AddListener(bufferedLog);

#ifdef WIN32
  logger.AddListener(std::make_shared<LogDebugOutputSink>());
#endif

  const int numberOfLogsPerThread = 5;

  // create a number of of log producers
  std::list<std::thread> producers;
  producers.push_back(std::thread([&logger]
  {
    MKL_LOGV(&logger, "test");
  }));
  producers.push_back(std::thread([&logger, numberOfLogsPerThread]
  {
    for (int i = 0; i < numberOfLogsPerThread; i++)
    {
      MKL_LOGD(&logger, "This is test {} in thread 0x{:x}", i, std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }
  }));
  producers.push_back(std::thread([&logger, numberOfLogsPerThread]
  {
    for (int i = 0; i < numberOfLogsPerThread; i++)
    {
      MKL_LOGI(&logger, "This is test {} in thread 0x{:x}", i, std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }
  }));
  producers.push_back(std::thread([&logger, numberOfLogsPerThread]
  {
    for (int i = 0; i < numberOfLogsPerThread; i++)
    {
      MKL_LOGW(&logger, "This is test {} in thread 0x{:x}", i, std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }
  }));
  producers.push_back(std::thread([&logger, numberOfLogsPerThread]
  {
    for (int i = 0; i < numberOfLogsPerThread; i++)
    {
      MKL_LOGE(&logger, "This is test {} in thread 0x{:x}", i, std::hash<std::thread::id>{}(std::this_thread::get_id()));
    }
  }));

  // wait for completion of all threads
  for (auto& t : producers)
  {
    t.join();
  }

  // wait until all items in the queue have been handled
  bufferedLog->Drain();

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
