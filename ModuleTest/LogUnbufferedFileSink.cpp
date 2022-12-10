module;

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

module LogUnbufferedFileSink;
import FileSystemUtils;

LogUnbufferedFileSink::~LogUnbufferedFileSink()
{
  Close();
}

bool LogUnbufferedFileSink::Create(const std::wstring& filePath)
{
  FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

  int fileDescriptor = 0;
  errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT | _O_TRUNC, _SH_DENYWR, _S_IWRITE);
  if (result == 0)
  {
    m_FileDescriptor = fileDescriptor;

    // write UTF-8 BOM
    const char* szUtf8Bom = "\xEF\xBB\xBF";
    _write(m_FileDescriptor, szUtf8Bom, 3);
  }
  return (result == 0);
}

bool LogUnbufferedFileSink::OpenToAppend(const std::wstring& filePath)
{
  int fileDescriptor = 0;
  errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT | _O_APPEND, _SH_DENYWR, _S_IWRITE);
  if (result == 0)
  {
    m_FileDescriptor = fileDescriptor;
  }
  return (result == 0);
}

void LogUnbufferedFileSink::Close()
{
  if (m_FileDescriptor != 0)
  {
    _close(m_FileDescriptor);
    m_FileDescriptor = 0;
  }
}

void LogUnbufferedFileSink::WriteToFile(const std::string& text)
{
  WriteToFile(text.data(), text.size() * sizeof(std::string::value_type));
}

void LogUnbufferedFileSink::WriteToFile(const void* data, size_t size)
{
  if (m_FileDescriptor != 0)
  {
    _write(m_FileDescriptor, data, static_cast<unsigned int>(size));
    _commit(m_FileDescriptor);
  }
}

// ILogSink
void LogUnbufferedFileSink::OutputRecord(const LogRecord& record)
{
  std::ostringstream buffer;
  OutputFormattedRecord(buffer, record);
  WriteToFile(buffer.str());
}
