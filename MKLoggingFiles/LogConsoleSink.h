#pragma once

#include "ILogSink.h"

class CLogConsoleSink : public ILogSink
{
public: // ILogSink
    void OutputString(const std::wstring& text) override;
};
