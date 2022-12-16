module;
#include <chrono>
#include <format>

module TimeModule;

std::string TimeAsIso8601String(const std::chrono::system_clock::time_point& tv)
{
  auto secondsTime = std::chrono::floor<std::chrono::seconds>(tv);
  return std::format("{:%Y-%m-%dT%H-%M-%S}", secondsTime);
}

std::wstring TimeAsIso8601WString(const std::chrono::system_clock::time_point& tv)
{
  auto secondsTime = std::chrono::floor<std::chrono::seconds>(tv);
  return std::format(L"{:%Y-%m-%dT%H-%M-%S}", secondsTime);
}

std::string LocalTimestampWithMillis(const std::chrono::system_clock::time_point& tv)
{
  // too slow: std::string s = std::format("{:%F %T}", tv);

  std::time_t t = std::chrono::system_clock::to_time_t(tv);
  char buf[20];
  strftime(buf, size_t(_countof(buf)), "%F %T", localtime(&t));
  std::string s(buf);

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tv.time_since_epoch()) % 1000;
  sprintf(buf, ".%03lli", ms.count());
  s += buf;
  return s;
}