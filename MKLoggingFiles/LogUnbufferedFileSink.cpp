#include "LogUnbufferedFileSink.h"
#include <io.h>
#include <fcntl.h>
#include <FileSystemUtils.h>

CLogUnbufferedFileSink::~CLogUnbufferedFileSink()
{
    Close();
}

bool CLogUnbufferedFileSink::Create(const std::wstring& filePath)
{
    FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

    int fileDescriptor = 0;
    errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT | _O_TRUNC, _SH_DENYWR, _S_IWRITE);
    if (result == 0) {
        m_FileDescriptor = fileDescriptor;
    }
    return (result == 0);
}

bool CLogUnbufferedFileSink::OpenToAppend(const std::wstring& filePath)
{
    int fileDescriptor = 0;
    errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT | _O_APPEND, _SH_DENYWR, _S_IWRITE);
    if (result == 0) {
        m_FileDescriptor = fileDescriptor;
    }
    return (result == 0);
}

void CLogUnbufferedFileSink::Close()
{
    if (m_FileDescriptor != 0) {
        _close(m_FileDescriptor);
        m_FileDescriptor = 0;
    }
}

void CLogUnbufferedFileSink::OutputString(const std::string& text)
{
    OutputData(text.data(), text.size() * sizeof(*text.data()));
}

//void CLogUnbufferedFileSink::OutputString(const std::wstring& text)
//{
//    // When the file is opened in Unicode translation mode—for example, if fd is opened by using _open or _sopen and a mode parameter that includes _O_WTEXT, _O_U16TEXT, or _O_U8TEXT, or if it's opened by using fopen and a mode parameter that includes ccs=UNICODE, ccs=UTF-16LE, or ccs=UTF-8, or if the mode is changed to a Unicode translation mode by using _setmode—buffer is interpreted as a pointer to an array of wchar_t that contains UTF-16 data. An attempt to write an odd number of bytes in this mode causes a parameter validation error.
//    OutputData(text.data(), text.size() * sizeof(*text.data()));
//}

void CLogUnbufferedFileSink::OutputData(const void* data, unsigned int size)
{
    if (m_FileDescriptor != 0) {
        _write(m_FileDescriptor, data, size);
        _commit(m_FileDescriptor);
    }
}
