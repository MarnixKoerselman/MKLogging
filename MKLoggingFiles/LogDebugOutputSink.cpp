#include "LogDebugOutputSink.h"
#include <Windows.h>

void CLogDebugOutputSink::OutputString(const std::wstring& text) noexcept
{
    if (IsDebuggerPresent()) {
        ::OutputDebugStringW(text.c_str());
    }
}
