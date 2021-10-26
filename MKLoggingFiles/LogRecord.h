#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <optional>

struct LogRecord
{
	LogRecord(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
	LogRecord(const LogRecord& rhs);

	std::ostream& Get();
	void LogHex(const char* szDataHeader, const void* data, int dataSize);
	std::string GetMessage() const;

	friend bool operator ==(const LogRecord& lhs, const LogRecord& rhs);

	ELogLevel LogLevel;
	std::string Function;
	std::string File;
	long LineNumber;
	std::chrono::system_clock::time_point Time;
	std::thread::id ThreadId;
	static uintptr_t ProcessId;
	std::optional<std::string> PreformattedMessage;

protected:
	std::ostringstream m_MessageBuffer;
};

