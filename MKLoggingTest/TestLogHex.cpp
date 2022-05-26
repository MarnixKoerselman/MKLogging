#include <Logger.h>
#include <Windows/LogDebugOutputSink.h>

#include <gtest/gtest.h>

TEST(Debug, LogHex)
{
  Logger logger({ std::make_shared<LogDebugOutputSink>() });
  char* data = "test";
  auto size = 5;
  MKL_LOGHEX(&logger, ELogLevel::Debug, "test", data, size);
}