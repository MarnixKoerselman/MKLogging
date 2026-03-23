#include "MKLogging/LogUnbufferedFileSink.h"
#include "MKLogging/FileSystemUtils.h"
#include <cstdio>
#include <string.h> // strerror

namespace MKLogging
{

  LogUnbufferedFileSink::~LogUnbufferedFileSink()
  {
    Close();
  }

  bool LogUnbufferedFileSink::Create(const std::filesystem::path& filePath)
  {
    FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

    Close();

    m_File = std::fopen(filePath.string().c_str(), "wb");
    if (m_File == nullptr)
    {
      std::string error = strerror(errno);
      return false;
    }
    std::setbuf(m_File, nullptr); // if BUF is NULL, make stream unbuffered
    // write UTF-8 BOM
    const char* szUtf8Bom = "\xEF\xBB\xBF";
    auto countWritten = std::fwrite(szUtf8Bom, sizeof(char), 3, m_File);
    return (3 == countWritten);
  }

  bool LogUnbufferedFileSink::OpenToAppend(const std::filesystem::path& filePath)
  {
    Close();
    m_File = std::fopen(filePath.string().c_str(), "ab");
    if (m_File != nullptr)
    {
      std::setbuf(m_File, nullptr); // if BUF is NULL, make stream unbuffered
    }
    return (m_File != nullptr);
  }

  void LogUnbufferedFileSink::Close()
  {
    if (m_File != nullptr)
    {
      std::fclose(m_File);
      m_File = nullptr;
    }
  }

  void LogUnbufferedFileSink::WriteToFile(const std::string& text)
  {
    WriteToFile(text.data(), text.size() * sizeof(std::string::value_type));
  }

  void LogUnbufferedFileSink::OutputRecord(const LogRecord& record)
  {
    WriteToFile(FormatRecord(record));
  }

  void LogUnbufferedFileSink::WriteToFile(const void* data, size_t size)
  {
    if (std::fwrite(data, size, 1, m_File) != size)
    {
      std::string error = strerror(errno);
    }
  }

} // namespace MKLogging
