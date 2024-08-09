#pragma once

#include "MKLogging/Logger.h"
#include <gmock/gmock.h>

class MockLogSink : public ILogSink
{
public: // ILogSink
  MOCK_METHOD1(OutputRecord, void(const LogRecord& record));
};
