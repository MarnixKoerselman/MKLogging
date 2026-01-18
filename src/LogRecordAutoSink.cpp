#include "MKLogging/LogRecordAutoSink.h"
#include <cassert>

namespace MKLogging
{

  LogRecordAutoSink::LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const std::string_view function, const std::string_view file, unsigned line)
    : LogRecord(logLevel, function, file, line)
    , m_LogSink(pLogSink)
  {
    assert(m_LogSink != nullptr);
  }

#if __cplusplus >= 202002L
  LogRecordAutoSink::LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, std::source_location location)
    : LogRecord(logLevel, location)
    , m_LogSink(pLogSink)
  {
    assert(m_LogSink != nullptr);
  }
#endif

  LogRecordAutoSink::~LogRecordAutoSink()
  {
    m_LogSink->OutputRecord(*this);
  }

} // namespace MKLogging
