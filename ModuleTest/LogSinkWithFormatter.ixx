export module LogSinkWithFormatter;

import ILogSink;
import LogFormatter;
import <memory>;

export class LogSinkWithFormatter : public ILogSink
{
public:
  LogSinkWithFormatter(bool useDefaultFormatter = false);
  virtual ~LogSinkWithFormatter() = default;
  void SetFormatter(const std::shared_ptr<LogFormatter>& formatter);
  void OutputFormattedRecord(std::ostream& os, const LogRecord& record);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override = 0;

protected:
  static const std::shared_ptr<LogFormatter>& GetDefaultFormatter();

protected:
  std::shared_ptr<LogFormatter> m_Formatter;
};
