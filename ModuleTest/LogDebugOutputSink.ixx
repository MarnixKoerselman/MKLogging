export module LogDebugOutputSink;
import LogSinkWithFormatter;

// Use OutputDebugString for output during debugging
export class LogDebugOutputSink : public LogSinkWithFormatter
{
public: // ILogSink
  void OutputRecord(const LogRecord& record) override;
};
