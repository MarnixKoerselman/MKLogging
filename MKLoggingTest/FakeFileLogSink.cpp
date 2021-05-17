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

void TestFileStream::OutputString(const std::string& text)
{
    m_LogFile << text;
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

void TestStdFile::PrintF(const wchar_t*szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);
    int length = _vscwprintf(szFormat, args);
    va_end(args);

    ASSERT_GE(length, 0);

    size_t nBufferLength = length + 1; // include terminating \00
    std::wstring sValue(nBufferLength, L'\00');

    va_start(args, szFormat);
    std::vswprintf(sValue.data(), sValue.size(), szFormat, args);
    va_end(args);

    std::fwprintf(m_File, sValue.c_str());
}
