#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>

class CLogStatement
{
public:
    CLogStatement(ILogSink* pLogSink);
    virtual ~CLogStatement();

    std::wostream& Get(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);

    void LogHex(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber, const char* szDataHeader, const void* pData, int iDataSize);

private:
    ILogSink* m_pLogSink;
    std::wostringstream m_Buffer;
};
