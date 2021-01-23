#include "LogConsoleSink.h"
#include <iostream>

void CLogConsoleSink::OutputString(const std::wstring& text)
{
    std::wcout << text;
}
