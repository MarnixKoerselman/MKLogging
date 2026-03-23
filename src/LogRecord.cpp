#include "MKLogging/LogRecord.h"
#include <chrono>
#include <cassert>
#include <thread>
#include <algorithm>
#include <cstring>
#include <format>

namespace MKLogging
{

  LogRecord::LogRecord(ELogLevel logLevel, const std::string_view function, const std::string_view file, unsigned line)
    : LogLevel(logLevel)
    , Function(function)
    , File(file)
    , Line(line)
    , Time(std::chrono::system_clock::now())
    , ThreadId(std::this_thread::get_id())
  {
  }

#if __cplusplus >= 202002L
  LogRecord::LogRecord(ELogLevel logLevel, std::source_location location)
    : LogLevel(logLevel)
    , Function(location.function_name())
    , File(location.file_name())
    , Line(location.line())
    , Time(std::chrono::system_clock::now())
    , ThreadId(std::this_thread::get_id())
  {
  }
#endif

  void LogRecord::SetMessage(std::string msg)
  {
    m_Message = std::move(msg);
  }

  void LogRecord::LogHex(const char* dataHeader, const void* data, int dataSize, int maxTraceValueCount /* = 128 */)
  {
    std::string result = std::format("{} Size={} bytes\n", dataHeader, dataSize);

    if ((data != nullptr) && (dataSize > 0))
    {
      const std::byte* pByteBuffer(reinterpret_cast<const std::byte*>(data));

      for (int i = 0; i < std::min(dataSize, maxTraceValueCount); i++)
      {
        result += std::format(" {:02X}", static_cast<int>(pByteBuffer[i]));
        if (((i + 1) % 16) == 0)
        {
          result += "\n";
        }
        else if (((i + 1) % 8) == 0)
        {
          result += "  ";
        }
      }
      if (dataSize > maxTraceValueCount)
      {
        result += " .......\n";
      }
      else
      {
        result += "\n";
      }
    }
    m_Message = std::move(result);
  }

  std::string LogRecord::GetLogMessage() const
  {
    if (PreformattedMessage)
    {
      return *PreformattedMessage;
    }
    return m_Message;
  }

  std::string LogRecord::UnformattedMessage() const
  {
    return m_Message;
  }

  bool operator ==(const LogRecord& lhs, const LogRecord& rhs)
  {
    return (lhs.LogLevel == rhs.LogLevel)
      && (lhs.File == rhs.File)
      && (lhs.Line == rhs.Line)
      && (lhs.Function == rhs.Function)
      && (lhs.Time == rhs.Time)
      && (lhs.ThreadId == rhs.ThreadId)
      && (lhs.m_Message == rhs.m_Message);
  }

} // namespace MKLogging
