#include "MKLogging/LogOutputStreamSink.h"
#include <iostream>

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
