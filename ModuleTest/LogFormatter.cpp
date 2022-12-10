module LogFormatter;
import TimeModule;

// a subclass can choose to override either the entrypoint, and/or the partials

void LogFormatter::OutputRecordWithFormatting(std::ostream& os, const LogRecord& record)
{
  auto flags = os.flags();
  os << record.File << '(' << record.LineNumber << ")\n";
  OutputTime(os, record.Time);
  os << ' ';
  OutputThreadId(os, record.ThreadId);
  os << ' ';
  OutputLogLevel(os, record.LogLevel);
  os << ' ';
  OutputMessage(os, record);
  os.flags(flags);
}

// partials
void LogFormatter::OutputLogLevel(std::ostream& os, ELogLevel logLevel)
{
  os << logLevel;
}

void LogFormatter::OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time)
{
  os << TimeAsLocalTimestampWithMillis(time);
}

void LogFormatter::OutputThreadId(std::ostream& os, std::thread::id threadId)
{
  os << "T=0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << threadId;
}

void LogFormatter::OutputMessage(std::ostream& os, const LogRecord& record)
{
  os << record.Function << ": " << record.GetLogMessage();
}
