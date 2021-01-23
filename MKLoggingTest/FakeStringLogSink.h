#pragma once

#include "Logger.h"

class FakeStringLogSink : public ILogSink
{
public:
    std::wstring Buffer;
public: // ILogSink
    virtual void OutputString(const std::wstring& text) override
    {
        Buffer.append(text);
    }
};
