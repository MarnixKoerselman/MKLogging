#pragma once

#include "ILogSink.h"

class CLogUtf8UnbufferedFileSink : public ILogSink
{
public:
    CLogUtf8UnbufferedFileSink() = default;
    virtual ~CLogUtf8UnbufferedFileSink();

    bool Create(const std::wstring& filePath);
    void Close();

public: // ILogSink
    void OutputString(const std::wstring& text) override;

private:
    int m_FileDescriptor = 0;
};

