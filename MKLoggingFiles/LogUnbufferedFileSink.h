#pragma once

#include "ILogSink.h"

class CLogUnbufferedFileSink : public ILogSink
{
public:
    CLogUnbufferedFileSink() = default;
    virtual ~CLogUnbufferedFileSink();

    bool Create(const std::wstring& filePath);
    bool OpenToAppend(const std::wstring& filePath);
    void Close();

public: // ILogSink
    void OutputString(const std::string& text) override;

private:
    void OutputData(const void* data, size_t size);
    int m_FileDescriptor = 0;
};

