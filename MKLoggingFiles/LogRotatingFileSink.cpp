#include "LogRotatingFileSink.h"
#include "Logger.h"
#include <map>
#include <regex>
#include <iomanip>

CLogRotatingFileSink::CLogRotatingFileSink(const std::filesystem::path& logFileDirectoryPath, long fileSizeThreshold /*= 10*1024*1024*/, int maxLogFileCount /*= 10*/)
    : m_LogFileDirectoryPath(logFileDirectoryPath)
    , m_FileSizeThreshold(fileSizeThreshold)
    , m_MaxFileCount(maxLogFileCount)
{
    if (m_LogFileDirectoryPath.empty())
    {
        LOGW(L"no output directory specified; log files will be placed in the current working directory");
    }
}

void CLogRotatingFileSink::OutputString(const std::string& text)
{
    RollOver();
    m_LogFile.OutputString(text);
}

void CLogRotatingFileSink::RollOver()
{
    // the 'current log file' may use caching methods, so the log file size must be tracked by the application and cannot be retrieved reliably from the file system
    if (m_LogFile.GetFileSize() >= m_FileSizeThreshold)
    {
        m_LogFile.Close();
    }

    if (!m_LogFile.IsOpen())
    {
        std::filesystem::path logFileName = GetNextFileName();
        LOGD(L"Create logfile " << logFileName);
        if (!m_LogFile.Create(m_LogFileDirectoryPath / logFileName))
        {
            // TODO: handle error
        }

        // keep the number of log files in check
        // make a collection of files, using the creation file stamp (from file name) to sort the collection
        // while collection size > m_MaxFileCount => remove the oldest file
        // the log files have the timestamp in sortable order in the file name
        std::list<std::filesystem::path> logFilePaths;
        std::wregex matcher(GetLogFileNameRegex());
        std::wcmatch wideMatch;

        for (auto& dirEntry : std::filesystem::directory_iterator(m_LogFileDirectoryPath))
        {
            if (std::regex_match(dirEntry.path().filename().c_str(), wideMatch, matcher))
            {
                LOGD(L"Found logfile " << dirEntry.path());
                logFilePaths.push_back(dirEntry.path());
            }
        }

        if (logFilePaths.size() > static_cast<size_t>(m_MaxFileCount))
        {
            logFilePaths.sort();
        }

        while (logFilePaths.size() > static_cast<size_t>(m_MaxFileCount))
        {
            const auto& filePath = logFilePaths.front();
            LOGD(L"Deleting logfile " << filePath);
            std::error_code resultCode;
            if (!std::filesystem::remove(filePath, resultCode))
            {
                LOGE(L"Failed to remove logfile " << filePath << L": " << resultCode.message().c_str());
            }
            logFilePaths.erase(logFilePaths.begin());
        }
    }
}

std::filesystem::path CLogRotatingFileSink::GetNextFileName() const
{
    std::filesystem::path logFileName = GenerateFileName();
    logFileName.replace_extension(m_LogFileExtension);
    return logFileName;
}

std::wstring CLogRotatingFileSink::GenerateFileName(time_t timeStamp /*= time(nullptr)*/) const
{
    tm gmtStamp;
    gmtime_s(&gmtStamp, &timeStamp);

    std::wstringstream fileNameStream;
    fileNameStream
        << m_sLogFileName
        << L'-'
        << std::put_time(&gmtStamp, L"%Y-%m-%d-%H-%M-%S");

    return fileNameStream.str();
}

std::wstring CLogRotatingFileSink::GetLogFileNameRegex() const
{
    // FormatString(L"%s-.*\\.%s", m_sLogFileName.c_str(), m_LogFileExtension.c_str())
    std::wstring matcher = m_sLogFileName;
    matcher += L"-.*\\.";
    matcher += m_LogFileExtension;
    return matcher;
}
