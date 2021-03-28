#pragma once

#include <string>

__interface ILogSink
{
    void OutputString(const std::string& text);
};

// Copy/paste:
//public: // ILogSink
//void OutputString(const std::string& text);
