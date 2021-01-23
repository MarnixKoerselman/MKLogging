#pragma once

#include "ILogSink.h"
#include <filesystem>

class CLogUtf8FileSink : public ILogSink
{
public:
    CLogUtf8FileSink() = default;
    virtual ~CLogUtf8FileSink();
    bool Create(const std::filesystem::path& directoryPath, const std::filesystem::path& fileName);
    void Close();

    long GetFileSize() const;
    bool IsOpen() const;

public: // ILogSink
    void OutputString(const std::wstring& text) override;

private:
    FILE* m_pFile = nullptr;
    long m_iWrittenByteCount = 0;
};
