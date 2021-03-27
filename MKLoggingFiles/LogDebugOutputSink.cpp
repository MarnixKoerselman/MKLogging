#include "LogDebugOutputSink.h"
#include <Windows.h>

void CLogDebugOutputSink::OutputString(const std::string& text) noexcept
{
    if (IsDebuggerPresent()) {
        ::OutputDebugStringA(text.c_str());
    }
}
