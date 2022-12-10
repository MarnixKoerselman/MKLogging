export module LogFormatter;
import ELogLevel;
import LogRecord;
import <iostream>;
import <chrono>;

// a subclass can choose to override either the entrypoint, and/or the partials
export class LogFormatter
{
public:
  // this method is the entrypoint, and calls the 'partials' below
  virtual void OutputRecordWithFormatting(std::ostream& os, const LogRecord& record);

  // partials
  virtual void OutputLogLevel(std::ostream& os, ELogLevel logLevel);
  virtual void OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time);
  virtual void OutputThreadId(std::ostream& os, std::thread::id threadId);
  virtual void OutputMessage(std::ostream& os, const LogRecord& record);
};
