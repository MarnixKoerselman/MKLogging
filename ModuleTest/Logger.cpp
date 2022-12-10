module;
#include "LogHelpers.h"

module Logger;
//import ELogLevel;
//import EventSource;
//import ILogSink;
//import LogSinkWithFormatter;
//import LogRecordAutoSink;
//import <iostream>;
//import <initializer_list>;
//import <sstream>;

Logger::Logger(std::initializer_list<std::shared_ptr<ILogSink>> sinks)
  : EventSource<ILogSink>(sinks)
  , LogSinkWithFormatter(true)
  , m_MinimumLogLevel(ELogLevel::All)
{
}

void Logger::SetMinimumLogLevel(ELogLevel logLevel)
{
  MKL_LOGW(this, ELogLevel_ToString(logLevel));
  m_MinimumLogLevel = logLevel;
}

void Logger::OutputRecord(const LogRecord& record)
{
  if (!IsLogged(record.LogLevel))
    return;

  // If there is a formatter registered, then prepare the preformatted message and store it in the record (N.B.: the 'parameter' record is not changed!)
  // The log sinks can decide for themselves if they want to use the preformatted message, or apply their own formatting.
  // If all sinks have a custom formatter, then the Logger's formatter should be nilled for performance reasons.
  const LogRecord* pRecord = &record;

  std::unique_ptr<LogRecord> recordWithPreformattedMessage;
  if (m_Formatter)
  {
    std::ostringstream buffer;
    OutputFormattedRecord(buffer, record);

    recordWithPreformattedMessage = std::make_unique<LogRecord>(record); // Make a temporary log record that contains the preformatted message. Don't change the 'source' record, that would mess with the responsibilities in the log chain.
    recordWithPreformattedMessage->PreformattedMessage = std::make_shared<std::string>(buffer.str());
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
  // TODO: allow overruling the hard-coded log level by dynamic application properties, to manipulate the log level at runtime

  return (logLevel >= m_MinimumLogLevel) && HasListeners();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// LogCentral: the central Logger instance.
// This global variable is instantiated automatically the first time it is used.
const std::shared_ptr<Logger>& SharedLogCentral()
{
  static auto g_LogCentral = std::make_shared<Logger>();
  return g_LogCentral;
}

Logger* LogCentral()
{
  return SharedLogCentral().get();
}
