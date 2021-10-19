#pragma once

#include "ILogSink.h"

class CLogConsoleSink : public LogSinkWithFormatter
{
public: // ILogSink
	void OutputRecord(const LogRecord& record) override;
};
