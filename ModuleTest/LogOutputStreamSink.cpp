module LogOutputStreamSink;

// NB: the owner of the ostream has the responsibility to keep it alive (instantiated) as long as the sink exists.
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
