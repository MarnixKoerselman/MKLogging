#include "FakeFileLogSink.h"
#include <gtest/gtest.h>
#include <cstdarg>

TestFileStream::TestFileStream(std::filesystem::path filePath)
{
  m_LogFile.open(filePath);
  EXPECT_TRUE(m_LogFile.good()) << L"Info: FilePath=" << filePath;
}

TestFileStream::~TestFileStream()
{
  m_LogFile.close();
}

std::ostream& TestFileStream::GetStream()
{
  return m_LogFile;
}

// ILogSink

void TestFileStream::OutputRecord(const LogRecord& record)
{
  if (record.PreformattedMessage)
  {
    m_LogFile << *record.PreformattedMessage;
  }
  else
  {
    m_LogFile << record.GetMessage();
  }
}

TestStdFile::TestStdFile(std::filesystem::path filePath)
{
  errno_t errorCode = _wfopen_s(&m_File, filePath.c_str(), L"w, ccs=UTF-8");
  EXPECT_EQ(0, errorCode);
}

TestStdFile::~TestStdFile()
{
  if (m_File != nullptr)
  {
    EXPECT_EQ(0, fclose(m_File));
    m_File = nullptr;
  }
}

void TestStdFile::PrintF(const wchar_t* szFormat, ...)
{
  va_list args;
  va_start(args, szFormat);
  std::vfwprintf(m_File, szFormat, args);
  va_end(args);
}
