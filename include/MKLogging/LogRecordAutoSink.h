#pragma once

#include "LogRecord.h"

namespace MKLogging
{

  struct LogRecordAutoSink : public LogRecord
  {
//#if __cplusplus >= 202002L
//    LogRecordAutoSink(ELogLevel logLevel, std::source_location location = std::source_location::current());
//#endif
    LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
    virtual ~LogRecordAutoSink();

  private:
    ILogSink* m_LogSink;
  };

} // namespace MKLogging
