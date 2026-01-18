#include <MKLogging/Logger.h>
#ifdef WIN32
#include "MKLogging/Windows/LogDebugOutputSink.h"
#endif
#include <MKLogging/LogOutputStreamSink.h>
#include <MKLogging/LogEnterLeave.h>
#include <gtest/gtest.h>
#include <source_location>

TEST(SourceLocationTest, LogRecordWithSourceLocation)
{
  auto record = MKLogging::LogRecord(MKLogging::ELogLevel::Info, std::source_location::current()); auto line = __LINE__;
  EXPECT_EQ(record.LogLevel, MKLogging::ELogLevel::Info);
  // this test is not going to work
  // 1. in both MSVC and GLIBCXX the implemetation of source_location differs from __FUNCTION__
  // 2. the implementation of __FUNCTION__ in MSVC differs vastly from the implementation in GLIBCXX
  // 3. for GLIBCXX the transition to source_location is an improvement, for MSVC it is not
  // EXPECT_STREQ(record.Function, __FUNCTION__);
  EXPECT_EQ(record.File, __FILE__);
  EXPECT_EQ(record.Line, line);
}

TEST(SourceLocationTest, LoggerWithSourceLocation)
{
#ifdef WIN32
  MKLogging::LogCentral()->AddListener(std::make_shared<MKLogging::LogDebugOutputSink>());
#endif
  MKLogging::LogCentral()->AddListener(std::make_shared<MKLogging::LogStdErrSink>());

  LOGV("test");
  LOG_ENTER_LEAVE(MKLogging::ELogLevel::Info);
}
