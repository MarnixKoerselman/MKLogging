#pragma once

#include "LogSinkWithFormatter.h"

class LogConsoleSink : public LogSinkWithFormatter
{
public: // ILogSink
	void OutputRecord(const LogRecord& record) override;
};
