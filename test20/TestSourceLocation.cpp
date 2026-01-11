#include <gtest/gtest.h>
#include <MKLogging/Logger.h>
#include <source_location>

using namespace MKLogging;

TEST(SourceLocationTest, LogRecordWithSourceLocation)
{
  auto record = LogRecord(ELogLevel::Info, std::source_location::current()); auto line = __LINE__;
  EXPECT_EQ(record.LogLevel, ELogLevel::Info);
  EXPECT_TRUE(record.Function.find(__FUNCTION__) != std::string_view::npos); // c++ function name is a bit more decorated (__cdecl) than __FUNCTION__
  EXPECT_EQ(record.File, __FILE__);
  EXPECT_EQ(record.LineNumber, line);
}