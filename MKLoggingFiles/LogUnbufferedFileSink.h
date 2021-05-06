#pragma once

#include "ILogSink.h"

class CLogUnbufferedFileSink : public ILogSink
{
public:
    CLogUnbufferedFileSink() = default;
    virtual ~CLogUnbufferedFileSink();

    bool Create(const std::wstring& filePath);
    void Close();

public: // ILogSink
    void OutputString(const std::string& text) override;

private:
    int m_FileDescriptor = 0;
};

