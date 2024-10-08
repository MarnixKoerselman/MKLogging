#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"

class Logger;

#ifndef MKL_LOG_ENTER_LEAVE
#define MKL_LOG_ENTER_LEAVE(mp_Logger, mp_LogLevel) LogEnterLeave mgen_LogEnterLeave(mp_Logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__);
#endif // !MKL_LOG_ENTER_LEAVE

#ifndef LOG_ENTER_LEAVE
#define LOG_ENTER_LEAVE(mp_LogLevel) MKL_LOG_ENTER_LEAVE(LogCentral(), mp_LogLevel)
#endif // !LOG_ENTER_LEAVE

class LogEnterLeave
{
public:
  LogEnterLeave(Logger* logger, ELogLevel logLevel, const char* function, const char* file, int line);
  virtual ~LogEnterLeave();

private:
  Logger* m_Logger;
  const ELogLevel m_LogLevel;
  const char* m_Function;
  const char* m_File;
  const long m_Line;
};

