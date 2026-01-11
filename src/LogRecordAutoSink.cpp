#include "MKLogging/LogRecordAutoSink.h"
#include <cassert>

namespace MKLogging
{

//#if __cplusplus >= 202002L
//  LogRecordAutoSink::LogRecordAutoSink(ELogLevel logLevel, std::source_location location)
//    : LogRecord(logLevel, location)
//    , m_LogSink(pLogSink)
//  {
//    assert(m_LogSink != nullptr);
//  }
//#endif

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

} // namespace MKLogging
