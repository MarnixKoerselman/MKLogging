#include "MKLogging/Logger.h"
#include "MKLogging/Version.h"
#include <format>

namespace MKLogging
{

  // Global instance; automatically instantiated for each application that references this static library.
  std::shared_ptr<Logger> SharedLogCentral()
  {
    static auto g_LogCentral = std::make_shared<Logger>();
    return g_LogCentral;
  }

  Logger* LogCentral()
  {
    return SharedLogCentral().get();
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////

  Logger::Logger(std::initializer_list<std::shared_ptr<ILogSink>> sinks)
    : EventSource<ILogSink>(sinks)
    , LogSinkWithFormatter(true)
    , m_MinimumLogLevel(ELogLevel::All)
  {
  }

  void Logger::SetMinimumLogLevel(ELogLevel logLevel)
  {
    MKL_LOGW(this, "Setting log level to: {}", ELogLevel_ToString(logLevel));
    m_MinimumLogLevel = logLevel;
  }

  void Logger::OutputRecord(const LogRecord& record)
  {
    if (!IsLogged(record.LogLevel))
      return;

    const LogRecord* pRecord = &record;

    std::unique_ptr<LogRecord> recordWithPreformattedMessage;
    if (m_Formatter)
    {
      std::string formatted = FormatRecord(record);

      recordWithPreformattedMessage = std::make_unique<LogRecord>(record);
      recordWithPreformattedMessage->PreformattedMessage = std::make_shared<std::string>(std::move(formatted));
      pRecord = recordWithPreformattedMessage.get();
    }

    Lock lock(m_AccessListeners);
    for (const auto& pListener : m_Listeners)
    {
      pListener->OutputRecord(*pRecord);
    }
  }

  bool Logger::IsLogged(ELogLevel logLevel)
  {
    return (logLevel >= m_MinimumLogLevel) && HasListeners();
  }

  std::string_view GetVersion()
  {
    return MKLOGGING_VERSION_STRING;
  }

} // namespace MKLogging
