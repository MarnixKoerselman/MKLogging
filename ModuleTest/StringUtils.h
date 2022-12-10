#pragma once

#include <string>

std::wstring FormatString(_In_z_ _Printf_format_string_ const wchar_t* szFormat, ...);
std::string FormatString(_In_z_ _Printf_format_string_ const char* szFormat, ...);

void StringReplaceAll(std::string& s, const std::string& sFrom, const std::string& sTo);

std::wstring Utf8ToUcs2(const std::string& text);

std::string Ucs2ToUtf8(const std::wstring& text);

