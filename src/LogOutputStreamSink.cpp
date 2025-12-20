#include "MKLogging/LogOutputStreamSink.h"
#include <iostream>

namespace MKLogging
{

	LogOutputStreamSink::LogOutputStreamSink(std::ostream& os)
		: m_Stream(os)
	{
	}

	void LogOutputStreamSink::OutputRecord(const LogRecord& record)
	{
		OutputFormattedRecord(m_Stream, record);
	}

	LogStdOutSink::LogStdOutSink()
		: LogOutputStreamSink(std::cout)
	{
	}

	LogStdErrSink::LogStdErrSink()
		: LogOutputStreamSink(std::cerr)
	{
	}

	LogStdLogSink::LogStdLogSink()
		: LogOutputStreamSink(std::clog)
	{
	}

} // namespace MKLogging
