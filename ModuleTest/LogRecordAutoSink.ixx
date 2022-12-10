export module LogRecordAutoSink;
import LogRecord;
import ILogSink;

export struct LogRecordAutoSink : public LogRecord
{
  LogRecordAutoSink(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
  LogRecordAutoSink(std::nullptr_t, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber) = delete;
  virtual ~LogRecordAutoSink();

private:
  ILogSink* m_LogSink;
};
