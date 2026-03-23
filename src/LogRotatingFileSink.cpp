#include "MKLogging/LogRotatingFileSink.h"
#include "MKLogging/Logger.h"
#include <regex>
#include <format>
#include <list>
#include <ctime>

namespace MKLogging
{

  LogRotatingFileSink::LogRotatingFileSink(const std::filesystem::path& logFileDirectoryPath, size_t fileSizeThreshold /*= 10*1024*1024*/, size_t maxLogFileCount /*= 10*/)
    : m_LogFileDirectoryPath(logFileDirectoryPath)
    , m_FileSizeThreshold(fileSizeThreshold)
    , m_MaxFileCount(maxLogFileCount)
  {
    if (m_LogFileDirectoryPath.empty())
    {
      LOGW("no output directory specified; log files will be placed in the current working directory");
    }
  }

  void LogRotatingFileSink::OutputRecord(const LogRecord& record)
  {
    RollOver();
    m_LogFile.OutputRecord(record);
  }

  void LogRotatingFileSink::RollOver()
  {
    // the 'current log file' may use caching methods, so the log file size must be tracked by the application and cannot be retrieved reliably from the file system
    if (m_LogFile.GetFileSize() >= m_FileSizeThreshold)
    {
      m_LogFile.Close();
    }

    if (!m_LogFile.IsOpen())
    {
      std::filesystem::path logFileName = GetNextFileName();
      LOGD("Create logfile {}", logFileName.string());
      if (!m_LogFile.Create(m_LogFileDirectoryPath / logFileName))
      {
        // TODO: handle error
      }

      // keep the number of log files in check
      std::list<std::filesystem::path> logFilePaths;
      std::wregex matcher(GetLogFileNameRegex());
      std::wcmatch wideMatch;

      for (auto& dirEntry : std::filesystem::directory_iterator(m_LogFileDirectoryPath))
      {
        if (std::regex_match(dirEntry.path().filename().wstring().c_str(), wideMatch, matcher))
        {
          LOGD("Found logfile {}", dirEntry.path().string());
          logFilePaths.push_back(dirEntry.path());
        }
      }

      if (logFilePaths.size() > m_MaxFileCount)
      {
        logFilePaths.sort();
      }

      while (logFilePaths.size() > m_MaxFileCount)
      {
        const auto& filePath = logFilePaths.front();
        LOGD("Deleting logfile {}", filePath.string());
        std::error_code resultCode;
        if (!std::filesystem::remove(filePath, resultCode))
        {
          LOGE("Failed to remove logfile {}: {}", filePath.string(), resultCode.message());
        }
        logFilePaths.erase(logFilePaths.begin());
      }
    }
  }

  std::filesystem::path LogRotatingFileSink::GetNextFileName() const
  {
    std::filesystem::path logFileName = GenerateFileName();
    logFileName.replace_extension(m_LogFileExtension);
    return logFileName;
  }

  std::wstring LogRotatingFileSink::GenerateFileName(const std::chrono::system_clock::time_point timeStamp) const
  {
    const auto currentDateTimeTimeT = std::chrono::system_clock::to_time_t(timeStamp);
    const auto currentDateTimeLocalTime = *std::gmtime(&currentDateTimeTimeT);

    // Use std::format for the narrow string part, then widen
    auto formatted = std::format("-{:04}-{:02}-{:02}-{:02}-{:02}-{:02}",
      currentDateTimeLocalTime.tm_year + 1900,
      currentDateTimeLocalTime.tm_mon + 1,
      currentDateTimeLocalTime.tm_mday,
      currentDateTimeLocalTime.tm_hour,
      currentDateTimeLocalTime.tm_min,
      currentDateTimeLocalTime.tm_sec);

    std::wstring result = m_sLogFileName;
    result.append(formatted.begin(), formatted.end());
    return result;
  }

  std::wstring LogRotatingFileSink::GetLogFileNameRegex() const
  {
    std::wstring matcher = m_sLogFileName;
    matcher += L"-.*\\.";
    matcher += m_LogFileExtension;
    return matcher;
  }

} // namespace MKLogging
