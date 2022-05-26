#pragma once

#include "LogSinkWithFormatter.h"

// Use OutputDebugString for output during debugging
class LogDebugOutputSink : public LogSinkWithFormatter
{
public: // ILogSink
  void OutputRecord(const LogRecord& record) override;
};
