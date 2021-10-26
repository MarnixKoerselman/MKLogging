#pragma once

#include "ILogSink.h"
#include <memory>
#include <ostream>

class LogSinkWithFormatter : public ILogSink
{
public:
  LogSinkWithFormatter(bool useDefaultFormatter = false);
  void SetFormatter(const std::shared_ptr<LogFormatter>& formatter);
  void OutputFormattedRecord(std::ostream& os, const LogRecord& record);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override = 0;

protected:
  std::shared_ptr<LogFormatter> m_Formatter;
  static std::shared_ptr<LogFormatter> DefaultFormatter;
};
