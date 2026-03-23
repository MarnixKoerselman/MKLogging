#pragma once

#include <format>
#include <string_view>

namespace MKLogging
{

  enum class ELogLevel
  {
    Verbose,  ///< Verbose debug logging.
    Debug,    ///< To be used while debugging.
    Info,     ///< Track progress of the application.
    Warning,  ///< Only warnings and more severe logging is written.
    Error,    ///< Only errors are logged.
    None,     ///< No logging.

    Min = Verbose,
    Max = None,
    All = Min,
  };

  const char* ELogLevel_ToString(ELogLevel logLevel);
  ELogLevel ELogLevel_FromString(const char* szLogLevel);

  void operator++(ELogLevel& logLevel);

} // namespace MKLogging

// std::format support for ELogLevel
template <>
struct std::formatter<MKLogging::ELogLevel> : std::formatter<std::string_view>
{
  auto format(MKLogging::ELogLevel level, std::format_context& ctx) const
  {
    return std::formatter<std::string_view>::format(MKLogging::ELogLevel_ToString(level), ctx);
  }
};
