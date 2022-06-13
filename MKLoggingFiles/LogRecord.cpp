#include "LogRecord.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>

LogRecord::LogRecord(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber)
  : LogLevel(logLevel)
  , Function(szFunction)
  , File(szFile)
  , LineNumber(lineNumber)
  , Time(std::chrono::system_clock::now())
  , ThreadId(std::this_thread::get_id())
{
  std::boolalpha(m_MessageBuffer);
}

LogRecord::LogRecord(const LogRecord& rhs)
  : LogLevel(rhs.LogLevel)
  , Function(rhs.Function)
  , File(rhs.File)
  , LineNumber(rhs.LineNumber)
  , Time(rhs.Time)
  , ThreadId(rhs.ThreadId)
  , PreformattedMessage(rhs.PreformattedMessage)
{
  m_MessageBuffer.str(rhs.m_MessageBuffer.str());
  std::boolalpha(m_MessageBuffer);
}

std::ostream& LogRecord::Get()
{
  return m_MessageBuffer;
}

void LogRecord::LogHex(const char* dataHeader, const void* data, int dataSize, int maxTraceValueCount /* = 128 */)
{
    std::ostream& os = Get();

    auto formatFlags = os.flags(); // remember the original stream formatting, to be restored when this szFunction is done

    os << dataHeader << " Size=" << dataSize << " bytes\n";

    if ((data != nullptr) && (dataSize > 0)) {
        const std::byte* pByteBuffer(reinterpret_cast<const std::byte*>(data));

        // show contents of the buffer as hex values
        os << std::hex << std::uppercase << std::setfill('0');
        for (int i = 0; i < std::min(dataSize, maxTraceValueCount); i++) {
            os << ' ' << std::setw(2) << static_cast<int>(pByteBuffer[i]);
            if (((i + 1) % 16) == 0) {
                os << "\n";
            }
            else if (((i + 1) % 8) == 0) {
                os << "  ";
            }
        }
        if (dataSize > maxTraceValueCount) {
            os << " .......\n";
        }
        else {
            os << "\n";
        }
    }
    os.flags(formatFlags);
}

std::string LogRecord::GetLogMessage() const
{
  return m_MessageBuffer.str();
}

bool operator ==(const LogRecord& lhs, const LogRecord& rhs)
{
  return (lhs.LogLevel == rhs.LogLevel)
    && (lhs.Function == rhs.Function)
    && (lhs.File == rhs.File)
    && (lhs.LineNumber == rhs.LineNumber)
    && (lhs.Time == rhs.Time)
    && (lhs.ThreadId == rhs.ThreadId)
    && (lhs.m_MessageBuffer.str() == rhs.m_MessageBuffer.str());
}
