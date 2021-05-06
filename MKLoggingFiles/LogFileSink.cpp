#include "LogFileSink.h"
#include "Logger.h"
#include <cassert>

CLogFileSink::~CLogFileSink()
{
    Close();
}

bool CLogFileSink::Create(const std::filesystem::path& directoryPath, const std::filesystem::path& fileName)
{
    LOGD(L"directory=" << directoryPath << L", fileName=" << fileName);

    // create the output directory if necessary
    if (!std::filesystem::exists(directoryPath))
    {
        if (!std::filesystem::create_directories(directoryPath))
        {
            LOGW(L"could not create output directory " << directoryPath);
        }
    }

    if (m_pFile != nullptr)
    {
        Close();
    }

    std::filesystem::path logFilePath = directoryPath / fileName;
    m_pFile = _wfsopen(logFilePath.c_str(), L"wt", _SH_DENYWR);
    // m_pFile = _wfsopen(logFilePath.c_str(), L"wt, ccs=UTF-8", _SH_DENYWR);
    return (m_pFile != nullptr);
}

void CLogFileSink::Close()
{
    if (m_pFile != nullptr)
    {
        const int iResult = fclose(m_pFile);
        m_pFile = nullptr;
        if (iResult != 0)
        {
            LOGE(L"fclose failed with error " << iResult);
        }
    }
}

long CLogFileSink::GetFileSize() const
{
    return m_iWrittenByteCount;
}

bool CLogFileSink::IsOpen() const
{
    return (m_pFile != nullptr);
}

// ILogSink
void CLogFileSink::OutputString(const std::string& text)
{
    assert(IsOpen());

    const int iWrittenByteCount = std::fprintf(m_pFile, text.c_str());
    if (iWrittenByteCount < 0)
    {
        LOGE(L"fwprintf failed");
    }
    else
    {
        m_iWrittenByteCount += iWrittenByteCount;
    }
}
