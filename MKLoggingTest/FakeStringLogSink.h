#pragma once

#include "Logger.h"

class FakeStringLogSink : public ILogSink
{
public:
    std::string Buffer;
public: // ILogSink
    virtual void OutputString(const std::string& text) override
    {
        Buffer.append(text);
    }
};
