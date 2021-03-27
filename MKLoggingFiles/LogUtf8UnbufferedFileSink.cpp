#include "LogUtf8UnbufferedFileSink.h"
#include <io.h>
#include <fcntl.h>

CLogUtf8UnbufferedFileSink::~CLogUtf8UnbufferedFileSink()
{
    Close();
}

bool CLogUtf8UnbufferedFileSink::Create(const std::wstring& filePath)
{
    int fileDescriptor = 0;
    errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_U8TEXT | _O_WRONLY | _O_CREAT, _SH_DENYWR, _S_IWRITE);
    if (result == 0) {
        m_FileDescriptor = fileDescriptor;
    }
    return (result == 0);
}

void CLogUtf8UnbufferedFileSink::Close()
{
    if (m_FileDescriptor != 0) {
        _close(m_FileDescriptor);
        m_FileDescriptor = 0;
    }
}

void CLogUtf8UnbufferedFileSink::OutputString(const std::string& text)
{
    if (m_FileDescriptor != 0) {
        _write(m_FileDescriptor, text.data(), static_cast<unsigned int>(text.size() * sizeof(char)));
        _commit(m_FileDescriptor);
    }
}
