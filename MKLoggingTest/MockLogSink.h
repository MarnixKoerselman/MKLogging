#pragma once

#include "Logger.h"
#include <gmock/gmock.h>

class MockLogSink : public ILogSink
{
public: // ILogSink
    MOCK_METHOD1(OutputString, void(const std::string& text));
};
