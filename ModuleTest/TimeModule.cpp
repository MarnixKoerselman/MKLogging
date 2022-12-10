module;
#include <format>
#include <chrono>

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

std::string TimeAsLocalTimestampWithMillis(const std::chrono::system_clock::time_point& tv)
{
  auto localTime = std::chrono::current_zone()->to_local(tv);
  auto milliTime = std::chrono::floor<std::chrono::milliseconds>(localTime);
  return std::format("{}", milliTime);
}