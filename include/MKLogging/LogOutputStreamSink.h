#pragma once

#include "LogSinkWithFormatter.h"
#include <ostream>

namespace MKLogging
{

  class LogOutputStreamSink : public LogSinkWithFormatter
  {
  public:
    LogOutputStreamSink(std::ostream& os); // NB: the owner of the ostream has the responsibility to keep it alive (instantiated) as long as the sink exists.

  public: // ILogSink
    void OutputRecord(const LogRecord& record) override;

  private:
    std::ostream& m_Stream;
  };

  class LogStdOutSink : public LogOutputStreamSink
  {
  public:
    LogStdOutSink();
  };

  class LogStdErrSink : public LogOutputStreamSink
  {
  public:
    LogStdErrSink();
  };

  class LogStdLogSink : public LogOutputStreamSink
  {
  public:
    LogStdLogSink();
  };

} // namespace MKLogging
