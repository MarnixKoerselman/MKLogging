module LogRecordAutoSink;

LogRecordAutoSink::LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber)
  : LogRecord(logLevel, szFunction, szFile, lineNumber)
  , m_LogSink(pLogSink)
{
}

LogRecordAutoSink::~LogRecordAutoSink()
{
  m_LogSink->OutputRecord(*this);
}
