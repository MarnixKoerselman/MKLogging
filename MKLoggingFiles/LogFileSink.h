#pragma once

#include "ILogSink.h"
//#include <filesystem>

class CLogFileSink : public ILogSink
{
public:
    CLogFileSink() = default;
    virtual ~CLogFileSink();
    bool Create(const std::wstring& filePath);
    void Close();

    size_t GetFileSize() const;
    bool IsOpen() const;

public: // ILogSink
    void OutputString(const std::string& text) override;

private:
    FILE* m_File = nullptr;
    size_t m_FileSize = 0;
};
