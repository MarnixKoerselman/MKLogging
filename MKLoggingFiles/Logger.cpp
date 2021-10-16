#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "Logger.h"
#include <codecvt>

// Global instance; automatically instantiated for each application that references this static library.
CLogCentral* LogCentral()
{
    static std::shared_ptr<CLogCentral> g_LogCentral = std::make_shared<CLogCentral>(std::initializer_list<std::shared_ptr<ILogSink>>{});
    return g_LogCentral.get();
}

std::ostream& operator <<(std::ostream& os, const std::wstring& s)
{
    return os << s.c_str();
}

std::ostream& operator <<(std::ostream& os, const wchar_t* sz)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return os << converter.to_bytes(sz);
}
