#include "MKLogging/LogFileSink.h"
#include "MKLogging/Logger.h"
#include "MKLogging/FileSystemUtils.h"

namespace MKLogging
{

  LogFileSink::~LogFileSink()
  {
    Close();
  }

  bool LogFileSink::Create(const std::filesystem::path& filePath)
  {
    LOGD("filePath={}", filePath.string());

    Close();

    // create the output directory if necessary
    FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

    // Open in untranslated mode
    m_File = std::fopen(filePath.string().c_str(), "wb");
    if (IsOpen())
    {
      const char* szUtf8Bom = "\xEF\xBB\xBF";
      m_FileSize = std::fwrite(szUtf8Bom, 1, 3, m_File);
    }

    return IsOpen();
  }

  void LogFileSink::Close()
  {
    if (IsOpen())
    {
      const int result = fclose(m_File);
      m_File = nullptr;
      if (result != 0)
      {
        LOGE("fclose failed with error {}", result);
      }
    }
  }

  size_t LogFileSink::GetFileSize() const
  {
    return m_FileSize;
  }

  bool LogFileSink::IsOpen() const
  {
    return (m_File != nullptr);
  }

  void LogFileSink::WriteToFile(const std::string& text)
  {
    if (IsOpen())
    {
      const size_t writtenElementCount = std::fwrite(text.data(), sizeof(std::string::value_type), text.size(), m_File);
      m_FileSize += writtenElementCount * sizeof(std::string::value_type);
    }
  }

  // ILogSink
  void LogFileSink::OutputRecord(const LogRecord& record)
  {
    if (IsOpen())
    {
      WriteToFile(FormatRecord(record));
    }
  }

} // namespace MKLogging
