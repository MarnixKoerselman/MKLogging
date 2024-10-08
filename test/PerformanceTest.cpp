#include "MKLogging/Logger.h"
#include "MKLogging/LogFileSink.h"
#include "MKLogging/LogQueue.h"

#include "TestUtils.h"

#pragma warning(push, 1)
#pragma warning (disable: 26495 26812)
#include "gtest/gtest.h"
#pragma warning(pop)

#include <filesystem>
#include <fstream>
#include "FakeFileLogSink.h"

class PerformanceTest : public testing::Test
{
public:
  void SetUp() override
  {
    ASSERT_FALSE(LogCentral()->HasListeners());
    LogCentral()->SetMinimumLogLevel(ELogLevel::Min);
  }

  void TearDown() override
  {
    LogCentral()->RemoveAllListeners();
    ASSERT_FALSE(LogCentral()->HasListeners());
  }

  void LogVerbose()
  {
    for (int i = SONG_LENGTH; i > 0; i--)
    {
      LOGV("There are " << i << " bottles of beer on the wall, " << i << " bottles of beer. Take one down and pass it around, " << i - 1 << " bottles of beer on the wall.");
    }
  }

  void LogDebug()
  {
    for (int i = SONG_LENGTH; i > 0; i--)
    {
      LOGD("There are " << i << " bottles of beer on the wall, " << i << " bottles of beer. Take one down and pass it around, " << i - 1 << " bottles of beer on the wall.");
    }
  }

  void LogInfo()
  {
    for (int i = SONG_LENGTH; i > 0; i--)
    {
      LOGI("There are " << i << " bottles of beer on the wall, " << i << " bottles of beer. Take one down and pass it around, " << i - 1 << " bottles of beer on the wall.");
    }
  }

  void LogWarning()
  {
    for (int i = SONG_LENGTH; i > 0; i--)
    {
      LOGW("There are " << i << " bottles of beer on the wall, " << i << " bottles of beer. Take one down and pass it around, " << i - 1 << " bottles of beer on the wall.");
    }
  }

  void LogError()
  {
    for (int i = SONG_LENGTH; i > 0; i--)
    {
      LOGE("There are " << i << " bottles of beer on the wall, " << i << " bottles of beer. Take one down and pass it around, " << i - 1 << " bottles of beer on the wall.");
    }
  }

  void Sing(int repetitionCount)
  {
    for (int i = 0; i < repetitionCount; i++)
    {
      LogVerbose();
      LogDebug();
      LogInfo();
      LogWarning();
      LogError();
    }
  }

  void Sing(int repetitionCount, int threadCount)
  {
    int repetitionsPerProducer = repetitionCount / threadCount;
    std::vector<std::thread> producers;
    for (int i = 0; i < threadCount; i++)
    {
      producers.push_back(std::thread([=]
        {
          Sing(repetitionsPerProducer);
        }));
    }
    for (auto& t : producers)
    {
      t.join();
    }
  }

  static const char* GetLogFileName()
  {
    return "PerformanceTest.log";
  }

  static std::filesystem::path GetLogFilePath(const char* testName)
  {
    return GetTestOutputDirectoryPath(testName) / GetLogFileName();
  }

  void SetUpLogWithAsynchronousFile(const std::filesystem::path& logFilePath)
  {
    EnsureCleanOutputDirectory(logFilePath.parent_path());

    auto fileSink = std::make_shared<LogFileSink>();
    ASSERT_TRUE(fileSink->Create(logFilePath));
    auto asyncFileSink = std::make_shared<LogQueue>(fileSink);
    LogCentral()->AddListener(asyncFileSink);
  }

  void VerifyLogFileEntries(const std::filesystem::path& logFilePath)
  {
    std::ifstream file(logFilePath);

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line))
    {
      if (!line.empty())
        lineCount += 1;
    }

    int numberOfLinesPerLogEntry = 2; // default formatter outputs 2 lines for each record
    EXPECT_EQ(static_cast<int>(ELogLevel::Max) * numberOfLinesPerLogEntry * SONG_LENGTH * REPETITION_COUNT, lineCount);
  }

  const int SONG_LENGTH = 100;
  const int SOME_PRODUCER_THREADS = 4;
  const int MANY_PRODUCER_THREADS = 10;
  const int REPETITION_COUNT = SOME_PRODUCER_THREADS * MANY_PRODUCER_THREADS * 2;
};

template<typename TimedFunction>
auto MeasureTime(TimedFunction func)
{
  std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
  func();
  std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  //std::cout << millis << " ms" << std::endl;
  return millis;
}

TEST_F(PerformanceTest, LogWithAsynchronousFile_ManyProducerThreads)
{
  auto logFilePath = GetLogFilePath("LogWithAsynchronousFile.ManyProducerThreads");
  SetUpLogWithAsynchronousFile(logFilePath);
  MeasureTime([this]()
    {
      Sing(REPETITION_COUNT, MANY_PRODUCER_THREADS);
      LogCentral()->RemoveAllListeners(); // drain queue & flush file
    });
  VerifyLogFileEntries(logFilePath);
}

TEST_F(PerformanceTest, FilePerformance)
{
  auto path = GetLogFilePath("PerformanceTest.FilePerformance");
  path.replace_filename(L"std.log");
  EnsureCleanOutputDirectory(path.parent_path());
  TestStdFile stdFile(path);
  path.replace_filename(L"stream.log");
  TestFileStream streamFile(path);

  int testCount = 10000;

  auto stdDuration = MeasureTime([this, &stdFile, testCount]()
    {
      for (int i = 0; i < testCount; i++)
      {
        stdFile.PrintF(L"This is test %i\n", i);
      }
    });

  auto streamDuration = MeasureTime([this, &streamFile, testCount]()
    {
      for (int i = 0; i < testCount; i++)
      {
        streamFile.GetStream() << L"This is test " << i << L"\n";
      }
    });

  GTEST_INFO("Standard file: " << stdDuration << " ms, stream: " << streamDuration << " ms\n");
}
