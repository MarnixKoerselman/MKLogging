#pragma once

#include <string>
#include <LogRecord.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <optional>

class LogFormatter
{
public:
	virtual void OutputRecordWithFormatting(std::ostream& os, const LogRecord& record);
	virtual void OutputLogLevel(std::ostream& os, ELogLevel logLevel);
	virtual void OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& time);
	virtual void OutputThreadId(std::ostream& os, std::thread::id threadId);
	virtual void OutputProcessId(std::ostream& os, uintptr_t processId);
};
