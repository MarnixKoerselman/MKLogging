#pragma once

#include <string>

__interface ILogSink
{
public: // ILogSink
    void OutputString(const std::string& text);
};
