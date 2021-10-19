#include "LogCentral.h"
#include "Logger.h"

CLogCentral::CLogCentral()
	: CEventSource<ILogSink>()
	, m_MinimumLogLevel(ELogLevel::Warning)
{
#ifdef _DEBUG
	m_MinimumLogLevel = ELogLevel::All;
#endif
}

void CLogCentral::SetMinimumLogLevel(ELogLevel logLevel)
{
	LOGW(ELogLevel_ToString(logLevel));
	m_MinimumLogLevel = logLevel;
}

void CLogCentral::OutputRecord(const LogRecord& record)
{
	const LogRecord* pRecord = &record;
	std::unique_ptr<LogRecord> recordWithPreformattedMessage;
	if (m_Formatter)
	{
		std::stringstream buffer;
		OutputFormattedRecord(buffer, record);

		recordWithPreformattedMessage = std::make_unique<LogRecord>(record);
		recordWithPreformattedMessage->PreformattedMessage = std::make_optional<std::string>(buffer.str());
		pRecord = recordWithPreformattedMessage.get();
	}
	Lock lock(m_AccessListeners);
	for (const auto& pListener : m_Listeners)
	{
		pListener->OutputRecord(*pRecord);
	}
}

bool CLogCentral::IsLogged(ELogLevel logLevel)
{
	// TODO: allow overruling the hard-coded log level by dynamic application properties, to manipulate the log level at runtime

	return (logLevel >= m_MinimumLogLevel) && HasListeners();
}
