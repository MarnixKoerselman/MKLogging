#pragma once

#include "Config.h"
#include "ILogSink.h"
#include "ELogLevel.h"
#include <string_view>

#if __cplusplus >= 202002L
#include <source_location>
#endif // __cplusplus >= 202002L

#ifndef MKL_LOG_ENTER_LEAVE
#if MKL_USE_SOURCE_LOCATION
#define MKL_LOG_ENTER_LEAVE(mp_Logger, mp_LogLevel) MKLogging::LogEnterLeave mgen_LogEnterLeave(mp_Logger, mp_LogLevel, std::source_location::current());
#else
#define MKL_LOG_ENTER_LEAVE(mp_Logger, mp_LogLevel) MKLogging::LogEnterLeave mgen_LogEnterLeave(mp_Logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__);
#endif // MKL_USE_SOURCE_LOCATION
#endif // MKL_LOG_ENTER_LEAVE

#ifndef LOG_ENTER_LEAVE
#define LOG_ENTER_LEAVE(mp_LogLevel) MKL_LOG_ENTER_LEAVE(MKLogging::LogCentral(), mp_LogLevel)
#endif // LOG_ENTER_LEAVE

namespace MKLogging
{

  class Logger;

  class LogEnterLeave
  {
  public:
    // C++17 compatible constructor - always available
    LogEnterLeave(Logger* logger, ELogLevel logLevel, const std::string_view function, const std::string_view file, unsigned line);
#if __cplusplus >= 202002L
    // C++20 constructor - only available when source_location is supported
    LogEnterLeave(Logger* logger, ELogLevel logLevel, std::source_location location);
#endif // __cplusplus >= 202002L
    virtual ~LogEnterLeave();

  private:
    Logger* m_Logger;
    const ELogLevel m_LogLevel;
    const std::string_view m_Function;
    const std::string_view m_File;
    const unsigned m_Line;
  };

} // namespace MKLogging
