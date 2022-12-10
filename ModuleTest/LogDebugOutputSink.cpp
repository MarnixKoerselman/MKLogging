module;
#pragma warning(push, 0)
#include <Windows.h>
#include <gsl/gsl>

module LogDebugOutputSink;
import <sstream>;

void LogDebugOutputSink::OutputRecord(const LogRecord& record)
{
  if (IsDebuggerPresent())
  {
    // UTF-8 text is not displayed correctly
    //::OutputDebugStringA(text.c_str());

    std::ostringstream buffer;
    OutputFormattedRecord(buffer, record);

    // the converter crashes if unexpected characters are passed in
    //static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    //OutputDebugStringW(converter.from_bytes(buffer.str()).c_str());

    // This is a Windows specific method anyway, so let's make use of Windows' transcoding facilities
    auto stringLength = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, buffer.str().c_str(), gsl::narrow_cast<int>(buffer.str().size()), nullptr, 0);
    std::wstring display;
    display.resize(stringLength + 1);
    stringLength = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, buffer.str().c_str(), gsl::narrow_cast<int>(buffer.str().size()), display.data(), gsl::narrow_cast<int>(display.size()));
    OutputDebugStringW(display.data());
  }
}
#pragma warning(pop)
