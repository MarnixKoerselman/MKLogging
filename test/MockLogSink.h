#pragma once

#include "MKLogging/Logger.h"
#include <gmock/gmock.h>

using namespace MKLogging;

class MockLogSink : public ILogSink
{
public: // ILogSink
  MOCK_METHOD1(OutputRecord, void(const LogRecord& record));
};
