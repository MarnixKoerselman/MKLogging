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

  void LogSinkWithFormatter::OutputFormattedRecord(std::ostream& os, const LogRecord& record)
  {
    if (m_Formatter)
    {
      m_Formatter->OutputRecordWithFormatting(os, record);
    }
    else if (record.PreformattedMessage)
    {
      os << *record.PreformattedMessage;
    }
    else
    {
      os << record.GetLogMessage();
    }
  }

  std::shared_ptr<LogFormatter> LogSinkWithFormatter::GetDefaultFormatter()
  {
    static std::shared_ptr<LogFormatter> g_DefaultFormatter = std::make_shared<LogFormatter>();
    return g_DefaultFormatter;
  }

} // namespace MKLogging
