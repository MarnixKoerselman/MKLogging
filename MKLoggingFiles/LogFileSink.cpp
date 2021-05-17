#include "LogFileSink.h"
#include "Logger.h"
#include "FileSystemUtils.h"

CLogFileSink::~CLogFileSink()
{
    Close();
}

bool CLogFileSink::Create(const std::wstring& filePath)
{
    LOGD(L"filePath=" << filePath.c_str());

    if (m_File != nullptr)
    {
        Close();
    }

    // create the output directory if necessary
    FileSystemUtils::CreateDirectoriesFromFilePath(filePath);
    m_File = _wfsopen(filePath.c_str(), L"wt", _SH_DENYWR);
    // m_File = _wfsopen(logFilePath.c_str(), L"wt, ccs=UTF-8", _SH_DENYWR);
    return (m_File != nullptr);
}

void CLogFileSink::Close()
{
    if (m_File != nullptr)
    {
        const int result = fclose(m_File);
        m_File = nullptr;
        if (result != 0)
        {
            LOGE(L"fclose failed with error " << result);
        }
    }
}

long CLogFileSink::GetFileSize() const
{
    return m_iWrittenByteCount;
}

bool CLogFileSink::IsOpen() const
{
    return (m_File != nullptr);
}

// ILogSink
void CLogFileSink::OutputString(const std::string& text)
{
    if (m_File == nullptr) {
        return;
    }

    const int writtenByteCount = std::fprintf(m_File, text.c_str());
    if (writtenByteCount < 0)
    {
        LOGE(L"fprintf failed");
    }
    else
    {
        m_iWrittenByteCount += writtenByteCount;
    }
}
