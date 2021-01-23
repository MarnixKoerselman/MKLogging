#include "LogDebugOutputSink.h"
#include <Windows.h>

void CLogDebugOutputSink::OutputString(const std::wstring& text) noexcept
{
    ::OutputDebugStringW(text.c_str());
}
