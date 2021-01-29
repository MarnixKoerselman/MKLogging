#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "StringUtils.h"
#include <cstdarg>
#include <locale>
#include <codecvt>

std::wstring FormatString(_In_z_ _Printf_format_string_ const wchar_t* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);
    const int iLength = _vscwprintf(szFormat, args);
    va_end(args);

    if (iLength < 0)
    {
        return std::wstring();
    }

    const size_t nBufferLength = size_t(iLength) + 1;
    std::wstring sValue(nBufferLength, L'\00'); // include terminating \00

    va_start(args, szFormat);
    std::vswprintf(sValue.data(), sValue.size(), szFormat, args);
    va_end(args);

    sValue.pop_back(); // remove terminating \00

    return sValue;
}

std::string FormatString(_In_z_ _Printf_format_string_ const char* szFormat, ...)
{
    va_list args;
    va_start(args, szFormat);
    const int iLength = _vscprintf(szFormat, args);
    va_end(args);

    if (iLength < 0)
    {
        return std::string();
    }

    const size_t nBufferLength = size_t(iLength) + 1;
    std::string sValue(nBufferLength, '\00'); // include terminating \00

    va_start(args, szFormat);
    std::vsnprintf(sValue.data(), sValue.size(), szFormat, args);
    va_end(args);

    sValue.pop_back(); // remove terminating \00

    return sValue;
}

void StringReplaceAll(std::wstring& s, const std::wstring& sFrom, const std::wstring& sTo)
{
    if (sFrom.empty())
        return;

    size_t nStartIndex = 0;
    while (nStartIndex = s.find(sFrom, nStartIndex), nStartIndex != std::wstring::npos)
    {
        s.replace(nStartIndex, sFrom.length(), sTo);
        nStartIndex += sTo.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

std::wstring Utf8ToUtf16(const std::string& utf8Str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(utf8Str);
}

std::string Utf16ToUtf8(const std::wstring& utf16Str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(utf16Str);
}
