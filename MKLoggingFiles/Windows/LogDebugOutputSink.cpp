#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "LogDebugOutputSink.h"
#include <Windows.h>
#include <codecvt>
#include <sstream>

void LogDebugOutputSink::OutputRecord(const LogRecord& record)
{
  if (IsDebuggerPresent())
  {
    // UTF-8 text is not displayed correctly
    //::OutputDebugStringA(text.c_str());

    std::ostringstream buffer;
    OutputFormattedRecord(buffer, record);

    static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    OutputDebugStringW(converter.from_bytes(buffer.str()).c_str());
  }
}
