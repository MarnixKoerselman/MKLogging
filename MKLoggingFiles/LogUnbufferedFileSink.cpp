#include "LogUnbufferedFileSink.h"
#include <io.h>
#include <fcntl.h>

CLogUnbufferedFileSink::~CLogUnbufferedFileSink()
{
    Close();
}

bool CLogUnbufferedFileSink::Create(const std::wstring& filePath)
{
    int fileDescriptor = 0;
    errno_t result = _wsopen_s(&fileDescriptor, filePath.c_str(), _O_BINARY | _O_WRONLY | _O_CREAT, _SH_DENYWR, _S_IWRITE);
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
    if (m_FileDescriptor != 0) {
        _write(m_FileDescriptor, text.data(), static_cast<unsigned int>(text.size() * sizeof(char)));
        _commit(m_FileDescriptor);
    }
}
