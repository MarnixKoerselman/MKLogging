#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <optional>

struct LogRecord
{
	LogRecord(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
	LogRecord(const LogRecord& rhs);
	virtual ~LogRecord();

	std::ostream& Get();

	void LogHex(const char* szDataHeader, const void* data, int dataSize);

	std::string GetMessage() const { return m_Buffer.str(); }

	friend bool operator ==(const LogRecord& lhs, const LogRecord& rhs)
	{
		return (lhs.LogLevel == rhs.LogLevel)
			&& (lhs.Function == rhs.Function)
			&& (lhs.File == rhs.File)
			&& (lhs.LineNumber == rhs.LineNumber)
			&& (lhs.Time == rhs.Time)
			&& (lhs.ThreadId == rhs.ThreadId)
			&& (lhs.ProcessId == rhs.ProcessId)
			&& (lhs.m_LogSink == rhs.m_LogSink)
			&& (lhs.m_Buffer.str() == rhs.m_Buffer.str());
	}

	ELogLevel LogLevel;
	std::string Function;
	std::string File;
	long LineNumber;
	std::chrono::system_clock::time_point Time;
	std::thread::id ThreadId;
	static uintptr_t ProcessId;
	std::optional<std::string> PreformattedMessage;

private:
	ILogSink* m_LogSink;
	std::ostringstream m_Buffer;
};
