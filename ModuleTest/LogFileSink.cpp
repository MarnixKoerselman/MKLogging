module;
#include "Logger.h"

module LogFileSink;
import FileSystemUtils;

LogFileSink::~LogFileSink()
{
  Close();
}
bool LogFileSink::Create(const std::wstring& filePath)
{
  LOGD(L"filePath=" << filePath);

  Close();

  // create the output directory if necessary
  FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

  // Open in untranslated mode
  m_File = _wfsopen(filePath.c_str(), L"wb", _SH_DENYWR);
  if (IsOpen())
  {
    const char* szUtf8Bom = "\xEF\xBB\xBF";
    m_FileSize = std::fwrite(szUtf8Bom, 1, 3, m_File);
  }

  // https ://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite
  // If file is opened with UTF-8 string processing, then only wstring values should be written
  // m_File = _wfsopen(filePath.c_str(), L"wt, ccs=UTF-8", _SH_DENYWR);

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
      LOGE(L"fclose failed with error " << result);
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

    //const size_t writtenElementCount = std::fwrite(text.data(), sizeof(std::wstring::value_type), text.size(), m_File);
    //m_FileSize += writtenElementCount * sizeof(std::wstring::value_type);
  }
}

// ILogSink
void LogFileSink::OutputRecord(const LogRecord& record)
{
  if (IsOpen())
  {
    std::ostringstream buffer;
    OutputFormattedRecord(buffer, record);
    WriteToFile(buffer.str());
  }
}
