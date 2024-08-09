#include "MKLogging/LogRecordAutoSink.h"
#include <cassert>

LogRecordAutoSink::LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber)
  : LogRecord(logLevel, szFunction, szFile, lineNumber)
  , m_LogSink(pLogSink)
{
  assert(m_LogSink != nullptr);
}

LogRecordAutoSink::~LogRecordAutoSink()
{
  m_LogSink->OutputRecord(*this);
}
