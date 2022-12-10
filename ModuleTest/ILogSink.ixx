export module ILogSink;

import LogRecord;

export class ILogSink
{
public:
  virtual void OutputRecord(const LogRecord& record) = 0;
};

// Copy/paste:
//public: // ILogSink
//void OutputRecord(const LogRecord& record) override;
