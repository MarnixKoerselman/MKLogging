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

std::wostream& TestFileStream::GetStream()
{
    return m_LogFile;
}

// ILogSink

void TestFileStream::OutputString(const std::wstring& text)
{
    m_LogFile << text;
}

TestStdFile::TestStdFile(std::filesystem::path filePath)
{
    errno_t errorCode = _wfopen_s(&m_pFile, filePath.c_str(), L"w, ccs=UTF-8");
    EXPECT_EQ(0, errorCode);
}

TestStdFile::~TestStdFile()
{
    if (m_pFile != nullptr)
    {
        EXPECT_EQ(0, fclose(m_pFile));
        m_pFile = nullptr;
    }
}

void TestStdFile::PrintF(const wchar_t*szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);
    int iLength = _vscwprintf(szFormat, args);
    va_end(args);

    ASSERT_GE(iLength, 0);

    size_t nBufferLength = iLength + 1; // include terminating \00
    std::wstring sValue(nBufferLength, L'\00');

    va_start(args, szFormat);
    std::vswprintf(sValue.data(), sValue.size(), szFormat, args);
    va_end(args);

    std::fwprintf(m_pFile, sValue.c_str());
}
