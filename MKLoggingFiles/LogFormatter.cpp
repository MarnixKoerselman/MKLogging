#include "LogFormatter.h"

void LogFormatter::OutputRecordWithFormatting(std::ostream& os, const LogRecord& record)
{
  os << record.File << "(" << record.LineNumber << ")\n";
  OutputTime(os, record.Time);
  OutputThreadId(os, record.ThreadId);
  OutputProcessId(os, record.ProcessId);
  OutputLogLevel(os, record.LogLevel);
  os << record.Function << ": " << record.GetLogMessage();
}

void LogFormatter::OutputLogLevel(std::ostream& os, ELogLevel logLevel)
{
  os << std::left << std::setw(10) << std::setfill(' ') << ELogLevel_ToString(logLevel);
}

void LogFormatter::OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time)
{
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;
  // convert to std::time_t in order to convert to std::tm (broken time)
  auto time2 = std::chrono::system_clock::to_time_t(time);
  tm localTime;
  if (localtime_s(&localTime, &time2) == 0)
  {
    os << std::put_time(&localTime, "%F %T") << "." << std::setw(3) << std::setfill('0') << ms.count();
  }
}

void LogFormatter::OutputThreadId(std::ostream& os, std::thread::id threadId)
{
  os << " T=0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << threadId << " " << std::dec;
}

void LogFormatter::OutputProcessId(std::ostream& os, uintptr_t processId)
{
  os << " P=0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << processId << " " << std::dec;
}
