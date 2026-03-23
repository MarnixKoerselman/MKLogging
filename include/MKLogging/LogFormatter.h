#pragma once

#include "LogRecord.h"
#include <chrono>
#include <thread>
#include <string>

namespace MKLogging
{

  // a subclass can choose to override either the entrypoint, and/or the partials
  class LogFormatter
  {
  public:
    virtual ~LogFormatter() = default;

    // this method is the entrypoint, and calls the 'partials' below
    virtual std::string FormatRecord(const LogRecord& record);
    // partials
    virtual std::string FormatLogLevel(ELogLevel logLevel);
    virtual std::string FormatTime(const std::chrono::system_clock::time_point& time);
    virtual std::string FormatThreadId(std::thread::id threadId);
    virtual std::string FormatLogMessage(const LogRecord& record);
  };

} // namespace MKLogging
