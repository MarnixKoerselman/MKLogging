#include "MKLogging/LogEnterLeave.h"
#include "MKLogging/Logger.h"

namespace MKLogging
{
  LogEnterLeave::LogEnterLeave(Logger* logger, ELogLevel logLevel, const char* file, int line, const char* function)
    : m_Logger(logger)
    , m_LogLevel(logLevel)
    , m_File(file)
    , m_Line(line)
    , m_Function(function)
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink(m_Logger, m_LogLevel, m_File, m_Line, m_Function).Get() << "Entering" << MKL_ENDLINE;
    }
  }

#if __cplusplus >= 202002L
  LogEnterLeave::LogEnterLeave(Logger* logger, ELogLevel logLevel, std::source_location location)
    : m_Logger(logger)
    , m_LogLevel(logLevel)
    , m_File(location.file_name())
    , m_Line(location.line())
    , m_Function(location.function_name())
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink(m_Logger, m_LogLevel, location).Get() << "Entering" << MKL_ENDLINE;
    }
  }
#endif // __cplusplus >= 202002L

  LogEnterLeave::~LogEnterLeave()
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink(m_Logger, m_LogLevel, m_File, m_Line, m_Function).Get() << "Leaving" << MKL_ENDLINE;
    }
  }

} // namespace MKLogging
