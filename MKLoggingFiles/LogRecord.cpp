#include "LogRecord.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>

uintptr_t LogRecord::ProcessId = 0;

LogRecord::LogRecord(ILogSink* pLogSink, ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber)
	: m_LogSink(pLogSink)
	, LogLevel(logLevel)
	, Function(szFunction)
	, File(szFile)
	, LineNumber(lineNumber)
	, Time(std::chrono::system_clock::now())
	, ThreadId(std::this_thread::get_id())
{
	assert(m_LogSink != nullptr);
	std::boolalpha(m_Buffer);
}

LogRecord::LogRecord(const LogRecord& rhs)
	: LogLevel(rhs.LogLevel)
	, Function(rhs.Function)
	, File(rhs.File)
	, LineNumber(rhs.LineNumber)
	, Time(rhs.Time)
	, ThreadId(rhs.ThreadId)
	, PreformattedMessage(rhs.PreformattedMessage)
	, m_LogSink(rhs.m_LogSink)
{
	m_Buffer.str(m_Buffer.str());
}

LogRecord::~LogRecord()
{
	m_LogSink->OutputRecord(*this);
}

std::ostream& LogRecord::Get()
{
	return m_Buffer;
	//    std::ostream& os = m_Buffer;
	//
	//#ifdef _DEBUG
	//    // Only show full file path (which may include personal info of the developer) in debug build
	//    if ((logLevel >= ELogLevel::Warning))
	//    {
	//        os << szFile << " (" << lineNumber << ")\n";
	//    }
	//#endif
	//
	//    auto now = std::chrono::system_clock::now();
	//    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	//    // convert to std::time_t in order to convert to std::tm (broken time)
	//    auto now2 = std::chrono::system_clock::to_time_t(now);
	//    tm systemTime;
	//    if (localtime_s(&systemTime, &now2) != 0)
	//    {
	//        //WTF? Could not convert to local time?
	//        std::cerr << "Cannot convert time_t " << now2 << " to local time, errno=" << errno << std::endl;
	//    }
	//    
	//    os << std::put_time(&systemTime, "%F %T") << "." << std::setw(3) << std::setfill('0') << ms.count()
	//        << " (0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << std::this_thread::get_id() << ") "
	//        << std::nouppercase << std::dec << std::left << std::setw(10) << std::setfill(' ')
	//        << ELogLevel_ToString(logLevel)
	//        << szFunction << ": ";
	//
	//    return os;
}

void LogRecord::LogHex(const char* szDataHeader, const void* data, int dataSize)
{
	std::ostream& os = Get();

	os << szDataHeader << " Size=" << dataSize << " bytes\n";

	if ((data != nullptr) && (dataSize > 0))
	{
		// trace a maximum of 128 values
		const int maxTraceValueCount = 128;
		const std::uint8_t* pByteBuffer(reinterpret_cast<const std::uint8_t*>(data));

		// show contents of the buffer as hex values
		os << std::hex << std::uppercase << std::setfill('0');
		for (int i = 0; i < std::min(dataSize, maxTraceValueCount); i++)
		{
			if (i > 0)
			{
				if ((i % 16) == 0)
				{
					os << "\n";
				}
				else if ((i % 8) == 0)
				{
					os << "  ";
				}
			}

			os << ' ' << std::setw(2) << pByteBuffer[i];
		}

		if (dataSize > maxTraceValueCount)
		{
			os << "\n...........";
			for (int i = dataSize - maxTraceValueCount; i < dataSize; i++)
			{
				int column = dataSize - i - maxTraceValueCount;
				if ((column % 16) == 0)
				{
					os << "\n";
				}
				else if ((column % 8) == 0)
				{
					os << "  ";
				}
				os << ' ' << std::setw(2) << pByteBuffer[i];
			}
		}
		os << "\n";
	}
}
