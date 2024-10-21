#pragma once

#include "ILogSink.h"
#include <memory>
#include <ostream>

class LogFormatter;

class LogSinkWithFormatter : public ILogSink
{
public:
  LogSinkWithFormatter(bool useDefaultFormatter = false);
  virtual ~LogSinkWithFormatter() = default;

  void SetFormatter(const std::shared_ptr<LogFormatter>& formatter);
  void OutputFormattedRecord(std::ostream& os, const LogRecord& record);

  static std::shared_ptr<LogFormatter> GetDefaultFormatter();

public: // ILogSink
  void OutputRecord(const LogRecord& record) override = 0;

protected:
  std::shared_ptr<LogFormatter> m_Formatter;
};
