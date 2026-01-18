#pragma once

#include "LogRecord.h"

namespace MKLogging
{

  struct LogRecordAutoSink : public LogRecord
  {
    // C++17 compatible constructor - always available
    LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* file, int line, const char* function);
#if __cplusplus >= 202002L
    // C++20 constructor - only available when source_location is supported
    LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, std::source_location location);
#endif
    virtual ~LogRecordAutoSink();

  private:
    ILogSink* m_LogSink;
  };

} // namespace MKLogging
