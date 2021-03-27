#include "LogConsoleSink.h"
#include <iostream>

void CLogConsoleSink::OutputString(const std::string& text)
{
    std::cout << text;
}
