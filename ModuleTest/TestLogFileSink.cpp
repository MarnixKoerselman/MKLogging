﻿import LogFileSink;

#include <gtest/gtest.h>
#include "TestUtils.h"

//#include "LogRecord.h"
//#include "LogFormatter.h"

TEST(LogFileSink, Basic)
{
  std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
  EnsureCleanOutputDirectory(testOuputDirectoryPath);

  std::filesystem::path logFilePath = testOuputDirectoryPath / "test.log";
  std::string testMessage = "Hello World\n";
  {
    LogFileSink logFile;
    ASSERT_TRUE(logFile.Create(logFilePath));
    logFile.WriteToFile(testMessage);
  }
  // the log file is now flushed and closed and ready for evaluation

  std::string actualOutput = ReadLogFileAsBinary(logFilePath);
  // skip the 3 byte BOM
  actualOutput.erase(0, 3);

  EXPECT_EQ(testMessage, actualOutput);
}

TEST(LogFileSink, Utf8)
{
  std::filesystem::path testOuputDirectoryPath = TEST_OUTPUT_DIRECTORY_PATH;
  EnsureCleanOutputDirectory(testOuputDirectoryPath);

  std::filesystem::path logFilePath = testOuputDirectoryPath / "test.log";
  std::u8string original = u8"Hello World\n你好世界";
  std::string testMessage(original.cbegin(), original.cend());
  {
    LogFileSink logFile;
    ASSERT_TRUE(logFile.Create(logFilePath));
    logFile.WriteToFile(testMessage);
  }
  // the log file is now flushed and closed and ready for evaluation

  std::string actualOutput = ReadLogFileAsBinary(logFilePath);
  // skip the 3 byte BOM
  actualOutput.erase(0, 3);

  EXPECT_EQ(testMessage, actualOutput);
}
