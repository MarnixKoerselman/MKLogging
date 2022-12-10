export module LogOutputStreamSink;
import LogSinkWithFormatter;

export class LogOutputStreamSink : public LogSinkWithFormatter
{
public:
  // NB: the owner of the ostream has the responsibility to keep it alive (instantiated) as long as the sink exists.
  LogOutputStreamSink(std::ostream& os);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override;

private:
  std::ostream& m_Stream;
};

export class LogStdOutSink : public LogOutputStreamSink
{
public:
  LogStdOutSink();
};

export class LogStdErrSink : public LogOutputStreamSink
{
public:
  LogStdErrSink();
};
