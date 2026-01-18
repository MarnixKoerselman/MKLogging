#include "MKLogging/LogRecord.h"
#include <chrono>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <cstring>

namespace MKLogging
{

  LogRecord::LogRecord(ELogLevel logLevel, const char* file, int line, const char* function)
    : LogLevel(logLevel)
    , File(file)
    , Line(line)
    , Function(function)
    , Time(std::chrono::system_clock::now())
    , ThreadId(std::this_thread::get_id())
  {
    std::boolalpha(m_MessageBuffer);
  }

#if __cplusplus >= 202002L
  LogRecord::LogRecord(ELogLevel logLevel, std::source_location location)
    : LogLevel(logLevel)
    , File(location.file_name())
    , Line(location.line())
    , Function(location.function_name())
    , Time(std::chrono::system_clock::now())
    , ThreadId(std::this_thread::get_id())
  {
    std::boolalpha(m_MessageBuffer);
  }
#endif

  LogRecord::LogRecord(const LogRecord& rhs)
    : LogLevel(rhs.LogLevel)
    , File(rhs.File)
    , Line(rhs.Line)
    , Function(rhs.Function)
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

    auto formatFlags = os.flags(); // remember the original stream formatting, to be restored when this function is done

    os << dataHeader << " Size=" << dataSize << " bytes\n";

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

  std::string LogRecord::GetLogMessage() const
  {
    if (PreformattedMessage)
    {
      return *PreformattedMessage;
    }
    return m_MessageBuffer.str();
  }

  std::string LogRecord::UnformattedMessage() const
  {
    return m_MessageBuffer.str();
  }

  bool operator ==(const LogRecord& lhs, const LogRecord& rhs)
  {
    return (lhs.LogLevel == rhs.LogLevel)
      && (strcmp(lhs.File, rhs.File) == 0)
      && (lhs.Line == rhs.Line)
      && (strcmp(lhs.Function, rhs.Function) == 0)
      && (lhs.Time == rhs.Time)
      && (lhs.ThreadId == rhs.ThreadId)
      && (lhs.m_MessageBuffer.str() == rhs.m_MessageBuffer.str());
  }

} // namespace MKLogging
