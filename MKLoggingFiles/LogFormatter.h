#pragma once

#include "LogRecord.h"
#include <chrono>
#include <thread>
#include <ostream>

// a subclass can choose to override either the entrypoint, and/or the partials
class LogFormatter
{
public:
  // this method is the entrypoint, and calls the 'partials' below
  virtual void OutputRecordWithFormatting(std::ostream& os, const LogRecord& record);
  // partials
  virtual void OutputLogLevel(std::ostream& os, ELogLevel logLevel);
  virtual void OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time);
  virtual void OutputThreadId(std::ostream& os, std::thread::id threadId);
  virtual void OutputProcessId(std::ostream& os, uint32_t processId);
};
