#pragma once

#include "LogRecord.h"

struct LogRecordAutoSink : public LogRecord
{
  LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
  virtual ~LogRecordAutoSink();

private:
  ILogSink* m_LogSink;
};
