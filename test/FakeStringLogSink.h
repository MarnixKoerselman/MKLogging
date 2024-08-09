#pragma once

#include "MKLogging/Logger.h"

class FakeStringLogSink : public ILogSink
{
public:
  std::string Buffer;
public: // ILogSink
  virtual void OutputRecord(const LogRecord& record) override
  {
    if (record.PreformattedMessage)
    {
      Buffer.append(*record.PreformattedMessage);
    }
    else
    {
      Buffer.append(record.GetLogMessage());
    }
  }
};
