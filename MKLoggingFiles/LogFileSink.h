#pragma once

#include "ILogSink.h"
//#include <filesystem>

class CLogFileSink : public ILogSink
{
public:
    CLogFileSink() = default;
    virtual ~CLogFileSink();
    //bool Create(const std::filesystem::path& directoryPath, const std::filesystem::path& fileName);
    bool Create(const std::wstring& filePath);
    void Close();

    long GetFileSize() const;
    bool IsOpen() const;

public: // ILogSink
    void OutputString(const std::string& text) override;

private:
    FILE* m_File = nullptr;
    long m_iWrittenByteCount = 0;
};
