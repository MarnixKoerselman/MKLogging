#include <Logger.h>
#include <LogDebugOutputSink.h>

#include <gtest/gtest.h>

TEST(Debug, LogHex)
{
    CLogCentral logger({std::make_shared<CLogDebugOutputSink>()});
    char* data = "test";
    auto size = 5;
    MKL_LOGHEX(&logger, ELogLevel::Debug, "test", data, size);
}