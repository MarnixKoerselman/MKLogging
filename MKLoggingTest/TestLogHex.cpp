#include <Logger.h>
#ifdef WIN32
#include <Windows/LogDebugOutputSink.h>
#endif
#include "MockLogSink.h"

#include <gtest/gtest.h>

TEST(Debug, LogHex)
{
  auto mockSink = std::make_shared<MockLogSink>();

#ifdef WIN32
  Logger logger({mockSink, std::make_shared<LogDebugOutputSink>()});
#else
  Logger logger({mockSink});
#endif

  const char* data = "test";
  auto size = 5;

  EXPECT_CALL(*mockSink, OutputRecord).WillOnce([](const LogRecord& record)
  {
    EXPECT_NE(record.GetLogMessage().find("74 65 73 74 00"), std::string::npos);
  }).RetiresOnSaturation();

  MKL_LOGHEX(&logger, ELogLevel::Debug, "test", data, size);
}
