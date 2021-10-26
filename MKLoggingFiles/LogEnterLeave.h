#pragma once

#include "ILogSink.h"
#include "Logger.h"

#define MKL_LOG_ENTER_LEAVE(mp_Logger, mp_LogLevel) LogEnterLeave mgen_logenterleave(mp_Logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__);

class LogEnterLeave
{
public:
  LogEnterLeave(Logger& logger, ELogLevel logLevel, const char* function, const char* file, int line);
  virtual ~LogEnterLeave();

private:
  Logger& m_Logger;
  ELogLevel m_LogLevel;
  const char* m_Function;
  const char* m_File;
  const long m_Line;
};

