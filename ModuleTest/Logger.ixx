export module Logger;
import ILogSink;
import EventSource;
import LogSinkWithFormatter;
import ELogLevel;

export class Logger : public EventSource<ILogSink>, public LogSinkWithFormatter
{
public:
  Logger(std::initializer_list<std::shared_ptr<ILogSink>> sinks = {});
  void SetMinimumLogLevel(ELogLevel logLevel);
  void OutputRecord(const LogRecord& record);
  bool IsLogged(ELogLevel logLevel);

private:
  ELogLevel m_MinimumLogLevel;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// LogCentral: the central Logger instance.
// This global variable is instantiated automatically the first time it is used.
export const std::shared_ptr<Logger>& SharedLogCentral();
export Logger* LogCentral();
