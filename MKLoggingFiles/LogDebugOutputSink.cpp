#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "LogDebugOutputSink.h"
#include <Windows.h>
#include <codecvt>

void CLogDebugOutputSink::OutputString(const std::string& text) noexcept
{
    if (IsDebuggerPresent()) {
        // UTF-8 text is not displayed correctly
        //::OutputDebugStringA(text.c_str());

        static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        OutputDebugStringW(converter.from_bytes(text).c_str());
    }
}
