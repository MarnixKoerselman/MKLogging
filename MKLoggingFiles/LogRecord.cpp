#include "LogRecord.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>

#define NOMINMAX
#include <Windows.h>

uintptr_t LogRecord::ProcessId = GetCurrentProcessId();

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
    && (lhs.ProcessId == rhs.ProcessId)
    && (lhs.m_MessageBuffer.str() == rhs.m_MessageBuffer.str());
}
