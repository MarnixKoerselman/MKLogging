#pragma once

namespace MKLogging
{

  struct LogRecord;

  class ILogSink
  {
  public: // ILogSink
    virtual void OutputRecord(const LogRecord& record) = 0;
  };

  // Copy/paste:
  //public: // ILogSink
  //void OutputRecord(const LogRecord& record) override;

} // namespace MKLogging
