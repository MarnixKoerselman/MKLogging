#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "MKLogging/LogFormatter.h"
#include <iomanip>
#include <time.h>

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

void LogFormatter::OutputLogLevel(std::ostream& os, ELogLevel logLevel)
{
  os << logLevel;
}

void LogFormatter::OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time)
{
  //auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;
  //// convert to std::time_t in order to convert to std::tm (broken time)
  //auto time2 = std::chrono::system_clock::to_time_t(time);
  //tm localTime;
  //if (localtime_s(&localTime, &time2) == 0)
  //{
  //  os << std::put_time(&localTime, "%F %T") << '.' << std::setw(3) << std::setfill('0') << std::dec << ms.count();
  //}
  // const auto currentDateTime = std::chrono::system_clock::now();
  const auto currentDateTime = time;
  const auto currentDateTimeTimeT = std::chrono::system_clock::to_time_t(currentDateTime);
  const auto currentDateTimeLocalTime = *std::gmtime(&currentDateTimeTimeT);

  const auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(currentDateTime).time_since_epoch().count() % 1000;
  os << std::put_time(&currentDateTimeLocalTime, "%F %T") << "." << std::setw(3) << std::setfill('0') << std::dec << ms;
}

void LogFormatter::OutputThreadId(std::ostream& os, std::thread::id threadId)
{
  os << "T=0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << threadId;
}

void LogFormatter::OutputMessage(std::ostream& os, const LogRecord& record)
{
  os << record.Function << ": " << record.GetLogMessage();
}
