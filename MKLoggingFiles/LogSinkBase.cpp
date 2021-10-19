#include "ILogSink.h"
#include "LogFormatter.h"

std::shared_ptr<LogFormatter> LogSinkWithFormatter::DefaultFormatter = std::make_shared<LogFormatter>();

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
		os << record.GetMessage();
	}
}

