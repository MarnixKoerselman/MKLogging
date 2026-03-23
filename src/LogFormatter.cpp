#include "MKLogging/LogFormatter.h"
#include <format>
#include <ctime>
#include <sstream>

namespace MKLogging
{

  std::string LogFormatter::FormatRecord(const LogRecord& record)
  {
    return std::format("{}({})\n{} {} {} {}",
      record.File, record.Line,
      FormatTime(record.Time),
      FormatThreadId(record.ThreadId),
      FormatLogLevel(record.LogLevel),
      FormatLogMessage(record));
  }

  std::string LogFormatter::FormatLogLevel(ELogLevel logLevel)
  {
    return std::format("{}", logLevel);
  }

  std::string LogFormatter::FormatTime(const std::chrono::system_clock::time_point& time)
  {
    const auto timeT = std::chrono::system_clock::to_time_t(time);
    const auto localTime = *std::gmtime(&timeT);
    const auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch().count() % 1000;
    return std::format("{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}",
      localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday,
      localTime.tm_hour, localTime.tm_min, localTime.tm_sec, ms);
  }

  std::string LogFormatter::FormatThreadId(std::thread::id threadId)
  {
    // std::thread::id doesn't have a direct integer accessor, use ostringstream for portability
    std::ostringstream oss;
    oss << threadId;
    unsigned long long id = 0;
    try { id = std::stoull(oss.str()); } catch (...) {}
    return std::format("T=0x{:08X}", id);
  }

  std::string LogFormatter::FormatLogMessage(const LogRecord& record)
  {
    return std::format("{}: {}", record.Function, record.GetLogMessage());
  }

} // namespace MKLogging
