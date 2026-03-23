#include "MKLogging/ILogSink.h"
#include "MKLogging/LogSinkWithFormatter.h"
#include "MKLogging/LogFormatter.h"

namespace MKLogging
{

  LogSinkWithFormatter::LogSinkWithFormatter(bool useDefaultFormatter)
  {
    if (useDefaultFormatter)
    {
      m_Formatter = GetDefaultFormatter();
    }
  }

  void LogSinkWithFormatter::SetFormatter(const std::shared_ptr<LogFormatter>& formatter)
  {
    m_Formatter = formatter;
  }

  std::string LogSinkWithFormatter::FormatRecord(const LogRecord& record)
  {
    if (m_Formatter)
    {
      return m_Formatter->FormatRecord(record);
    }
    else if (record.PreformattedMessage)
    {
      return *record.PreformattedMessage;
    }
    else
    {
      return record.GetLogMessage();
    }
  }

  std::shared_ptr<LogFormatter> LogSinkWithFormatter::GetDefaultFormatter()
  {
    static std::shared_ptr<LogFormatter> g_DefaultFormatter = std::make_shared<LogFormatter>();
    return g_DefaultFormatter;
  }

} // namespace MKLogging
