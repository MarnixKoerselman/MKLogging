#define NOMINMAX

#include "LogStatement.h"
#include <chrono>
#include <Windows.h>
#include <cassert>
#include <iostream>
#include <iomanip>

CLogStatement::CLogStatement(ILogSink* pLogSink)
    : m_pLogSink(pLogSink)
{
    assert(m_pLogSink != nullptr);
    std::boolalpha(m_Buffer);
}

CLogStatement::~CLogStatement()
{
    m_pLogSink->OutputString(m_Buffer.str());
}

std::wostream& CLogStatement::Get(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber)
{
    std::wostream& os = m_Buffer;

#ifdef _DEBUG
    // Only show full file path (which may include personal info of the developer) in debug build
    if (logLevel >= ELogLevel::Warning)
    {
        os << szFile << L" (" << lineNumber << L")\n";
    }
#else
    UNREFERENCED_PARAMETER(szFile);
    UNREFERENCED_PARAMETER(lineNumber);
#endif

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    // convert to std::time_t in order to convert to std::tm (broken time)
    auto now2 = std::chrono::system_clock::to_time_t(now);
    tm systemTime;
    if (localtime_s(&systemTime, &now2) != 0)
    {
        //WTF? Could not convert to local time?
        std::cerr << "Cannot convert time_t " << now2 << " to local time, errno=" << errno << std::endl;
    }
    os << std::put_time(&systemTime, L"%F %T") << L"." << std::setw(3) << std::setfill(L'0') << ms.count()
        << L" (0x" << std::hex << std::setw(8) << std::setfill(L'0') << std::uppercase << ::GetCurrentThreadId() << L") "
        << std::nouppercase << std::dec << std::left << std::setw(10) << std::setfill(L' ')
        << ELogLevel_ToString(logLevel)
        << szFunction << L": ";

    return os;
}

void CLogStatement::LogHex(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber, const char* szDataHeader, const void* pData, int iDataSize)
{
    std::wostream& os = Get(logLevel, szFunction, szFile, lineNumber);

    os << szDataHeader << L" Size=" << iDataSize << L" bytes\n";

    if ((pData != nullptr) && (iDataSize > 0))
    {
        // trace a maximum of 128 values
        const int maxTraceValueCount = 128;
        const std::uint8_t* pByteBuffer(reinterpret_cast<const std::uint8_t*>(pData));

        // show contents of the buffer as hex values
        os << std::hex << std::uppercase << std::setfill(L'0');
        for (int i = 0; i < std::min(iDataSize, maxTraceValueCount); i++)
        {
            if (i > 0)
            {
                if ((i % 16) == 0)
                {
                    os << L"\n";
                }
                else if ((i % 8) == 0)
                {
                    os << L"  ";
                }
            }

            os << L' ' << std::setw(2) << pByteBuffer[i];
        }

        if (iDataSize > maxTraceValueCount)
        {
            os << L"\n...........";
            for (int i = iDataSize - maxTraceValueCount; i < iDataSize; i++)
            {
                int column = iDataSize - i - maxTraceValueCount;
                if ((column % 16) == 0)
                {
                    os << L"\n";
                }
                else if ((column % 8) == 0)
                {
                    os << L"  ";
                }
                os << L' ' << std::setw(2) << pByteBuffer[i];
            }
        }
        os << std::endl;
    }
}
