#pragma once

#include "ILogSink.h"
//#include <filesystem>
#include <string>

class CLogFileSink : public LogSinkWithFormatter
{
public:
	CLogFileSink() = default;
	virtual ~CLogFileSink();
	bool Create(const std::wstring& filePath);
	void Close();

	size_t GetFileSize() const;
	bool IsOpen() const;

	void WriteToFile(const std::string& text);

public: // ILogSink
	void OutputRecord(const LogRecord& record) override;

private:
	FILE* m_File = nullptr;
	size_t m_FileSize = 0;
};
