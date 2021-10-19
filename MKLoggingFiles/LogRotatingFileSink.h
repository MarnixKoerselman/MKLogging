#pragma once

#include "ILogSink.h"
#include "LogFileSink.h"
#include <time.h>
#include <filesystem>

// Take care: only use this logger if it is used asynchronous, e.g. through CLogQueue. Using this directly (i.e. logging to this logger in the main thread)
// will most likely end up in infinite recursiveness, with a stack overflow as the fatal result.
class CLogRotatingFileSink : public LogSinkWithFormatter
{
public:
	CLogRotatingFileSink(const std::filesystem::path& logFileDirectoryPath, size_t logFileSizeThreshold = 10 * 1024 * 1024, int maxLogFileCount = 10);
	virtual ~CLogRotatingFileSink() = default;

public: // ILogSink
	void OutputRecord(const LogRecord& record) override;

protected: // methods made protected instead of private to improve testability
	void RollOver();
	std::filesystem::path GetNextFileName() const;
	std::wstring GenerateFileName(time_t now = time(nullptr)) const;
	std::wstring GetLogFileNameRegex() const;

private:
	const std::filesystem::path m_LogFileDirectoryPath;
	const std::wstring m_sLogFileName = L"app"; // The base name of the log file(s). The timestamp of file creation will be inserted in the name, and ".log" appended. NB: should not contain a '.' (because used in regex)
	const std::wstring m_LogFileExtension = L"log"; // NB: should not contain a '.' (because used in regex)
	CLogFileSink m_LogFile;
	const size_t m_FileSizeThreshold;
	const int m_MaxFileCount;
};
