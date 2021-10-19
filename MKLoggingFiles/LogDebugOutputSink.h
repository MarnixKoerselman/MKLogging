#pragma once

#include "ILogSink.h"

// Use OutputDebugString for output during debugging
class CLogDebugOutputSink : public LogSinkWithFormatter
{
public: // ILogSink
	void OutputRecord(const LogRecord& record) override;
};
