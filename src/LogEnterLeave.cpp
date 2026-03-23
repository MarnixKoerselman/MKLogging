#include "MKLogging/LogEnterLeave.h"
#include "MKLogging/Logger.h"

namespace MKLogging
{
  LogEnterLeave::LogEnterLeave(Logger* logger, ELogLevel logLevel, const std::string_view function, const std::string_view file, unsigned line)
    : m_Logger(logger)
    , m_LogLevel(logLevel)
    , m_Function(function)
    , m_File(file)
    , m_Line(line)
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink __rec(m_Logger, m_LogLevel, m_Function, m_File, m_Line);
      __rec.SetMessage("Entering\n");
    }
  }

#if __cplusplus >= 202002L
  LogEnterLeave::LogEnterLeave(Logger* logger, ELogLevel logLevel, std::source_location location)
    : m_Logger(logger)
    , m_LogLevel(logLevel)
    , m_Function(location.function_name())
    , m_File(location.file_name())
    , m_Line(location.line())
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink __rec(m_Logger, m_LogLevel, location);
      __rec.SetMessage("Entering\n");
    }
  }
#endif // __cplusplus >= 202002L

  LogEnterLeave::~LogEnterLeave()
  {
    if (MKL_ISLOGGED(m_Logger, m_LogLevel))
    {
      LogRecordAutoSink __rec(m_Logger, m_LogLevel, m_Function, m_File, m_Line);
      __rec.SetMessage("Leaving\n");
    }
  }

} // namespace MKLogging
