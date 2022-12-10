export module TimeModule;
import <string>;
import <chrono>;

export std::string TimeAsIso8601String(const std::chrono::system_clock::time_point& tv);
export std::wstring TimeAsIso8601WString(const std::chrono::system_clock::time_point& tv);
export std::string TimeAsLocalTimestampWithMillis(const std::chrono::system_clock::time_point& tv);
