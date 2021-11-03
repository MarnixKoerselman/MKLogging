#include "LogStatement.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>

CLogStatement::CLogStatement(ILogSink* pLogSink)
    : m_LogSink(pLogSink)
{
    assert(m_LogSink != nullptr);
    std::boolalpha(m_Buffer);
}

CLogStatement::~CLogStatement()
{
    m_LogSink->OutputString(m_Buffer.str());
}

std::ostream& CLogStatement::Get(ELogLevel logLevel, const char* szFunction, [[maybe_unused]] const char* szFile, [[maybe_unused]] long lineNumber)
{
    std::ostream& os = m_Buffer;

#ifdef _DEBUG
    // Only show full file path (which may include personal info of the developer) in debug build
    if ((logLevel >= ELogLevel::Warning))
    {
        os << szFile << " (" << lineNumber << ")\n";
    }
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
    
    os << std::put_time(&systemTime, "%F %T") << "." << std::setw(3) << std::setfill('0') << ms.count()
        << " (0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << std::this_thread::get_id() << ") "
        << std::nouppercase << std::dec << std::left << std::setw(10) << std::setfill(' ')
        << ELogLevel_ToString(logLevel)
        << szFunction << ": ";

    return os;
}

void CLogStatement::LogHex(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber, const char* szDataHeader, const void* data, int dataSize, int maxTraceValueCount /* = 128 */)
{
    std::ostream& os = Get(logLevel, szFunction, szFile, lineNumber);

    auto formatFlags = os.flags(); // remember the original stream formatting, to be restored when this szFunction is done

    os << szDataHeader << " Size=" << dataSize << " bytes\n";

    if ((data != nullptr) && (dataSize > 0))
    {
        const std::byte* pByteBuffer(reinterpret_cast<const std::byte*>(data));

        // show contents of the buffer as hex values
        os << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < std::min(dataSize, maxTraceValueCount); i++)
        {
            os << ' ' << std::setw(2) << static_cast<int>(pByteBuffer[i]);
            if (((i + 1) % 16) == 0)
            {
                os << "\n";
            }
            else if (((i + 1) % 8) == 0)
            {
                os << "  ";
            }
        }
        if (dataSize > maxTraceValueCount)
        {
            os << " .......\n";
        }
        else
        {
            os << "\n";
        }
    }
    os.flags(formatFlags);
}
