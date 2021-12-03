#pragma once

struct LogRecord;

__interface ILogSink
{
  void OutputRecord(const LogRecord& record);
};

// Copy/paste:
//public: // ILogSink
//void OutputRecord(const LogRecord& record) override;

