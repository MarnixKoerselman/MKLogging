#include "LogEnterLeave.h"

LogEnterLeave::LogEnterLeave(Logger* logger, ELogLevel logLevel, const char* function, const char* file, int line)
  : m_Logger(logger)
  , m_LogLevel(logLevel)
  , m_Function(function)
  , m_File(file)
  , m_Line(line)
{
  if (MKL_ISLOGGED(m_Logger, m_LogLevel))
  {
    LogRecordAutoSink(m_Logger, m_LogLevel, m_Function, m_File, m_Line).Get() << "Entering" << MKL_ENDLINE;
  }
}

LogEnterLeave::~LogEnterLeave()
{
  if (MKL_ISLOGGED(m_Logger, m_LogLevel))
  {
    LogRecordAutoSink(m_Logger, m_LogLevel, m_Function, m_File, m_Line).Get() << "Leaving" << MKL_ENDLINE;
  }
}
