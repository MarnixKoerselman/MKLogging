#include "LogUnbufferedFileSink.h"
#include "FileSystemUtils.h"
#include <sstream>
#include <cstdio>
#include <fstream>
#include <string.h> // strerror

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
  std::ostringstream buffer;
  OutputFormattedRecord(buffer, record);
  WriteToFile(buffer.str());
}

//void LogUnbufferedFileSink::OutputString(const std::wstring& text)
//{
//    // When the file is opened in Unicode translation mode—for example, if fd is opened by using _open or _sopen and a mode parameter that includes _O_WTEXT, _O_U16TEXT, or _O_U8TEXT, or if it's opened by using fopen and a mode parameter that includes ccs=UNICODE, ccs=UTF-16LE, or ccs=UTF-8, or if the mode is changed to a Unicode translation mode by using _setmode—buffer is interpreted as a pointer to an array of wchar_t that contains UTF-16 data. An attempt to write an odd number of bytes in this mode causes a parameter validation error.
//    OutputData(text.data(), text.size() * sizeof(std::wstring::value_type));
//}

void LogUnbufferedFileSink::WriteToFile(const void* data, size_t size)
{
  if (std::fwrite(data, size, 1, m_File) != size)
  {
    std::string error = strerror(errno);
  }
}
