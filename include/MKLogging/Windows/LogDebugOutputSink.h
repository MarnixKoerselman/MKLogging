#pragma once

#include "../LogSinkWithFormatter.h"

namespace MKLogging
{

  // Use OutputDebugString for output during debugging
  class LogDebugOutputSink : public LogSinkWithFormatter
  {
  public: // ILogSink
    void OutputRecord(const LogRecord& record) override;
  };

} // namespace MKLogging
