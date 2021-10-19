#pragma once

#include "ILogSink.h"
#include <string>

class CLogUnbufferedFileSink : public LogSinkWithFormatter
{
public:
	CLogUnbufferedFileSink() = default;
	virtual ~CLogUnbufferedFileSink();

	bool Create(const std::wstring& filePath);
	bool OpenToAppend(const std::wstring& filePath);
	void Close();

	void WriteToFile(const std::string& text);
	void WriteToFile(const void* data, size_t size);

public: // ILogSink
	void OutputRecord(const LogRecord& record) override;

private:
	int m_FileDescriptor = 0;
};

