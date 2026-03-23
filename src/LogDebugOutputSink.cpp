#include "MKLogging/Windows/LogDebugOutputSink.h"
#include <Windows.h>

#define NOMINMAX

namespace MKLogging
{

void LogDebugOutputSink::OutputRecord(const LogRecord& record)
{
  if (IsDebuggerPresent())
  {
    std::string formatted = FormatRecord(record);

    // This is a Windows specific method anyway, so let's make use of Windows' transcoding facilities
    auto stringLength = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, formatted.c_str(), static_cast<int>(formatted.size()), nullptr, 0);
    std::wstring display;
    display.resize(stringLength + 1);
    stringLength = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, formatted.c_str(), static_cast<int>(formatted.size()), display.data(), static_cast<int>(display.size()));
    OutputDebugStringW(display.data());
  }
}

} // namespace MKLogging
