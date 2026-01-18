#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <memory>
#if __cplusplus >= 202002L
#include <source_location>
#endif // __cplusplus >= 202002L

namespace MKLogging
{

  struct LogRecord
  {
    LogRecord(ELogLevel logLevel, const char* file, int line, const char* function);
#if __cplusplus >= 202002L
    LogRecord(ELogLevel logLevel, std::source_location location);
#endif // __cplusplus >= 202002L
    LogRecord(const LogRecord& rhs);
    virtual ~LogRecord() = default;

    std::ostream& Get();
    void LogHex(const char* dataHeader, const void* data, int dataSize, int maxTraceValueCount = 128);
    std::string GetLogMessage() const;

    friend bool operator ==(const LogRecord& lhs, const LogRecord& rhs);

    ELogLevel LogLevel;
    const char* File = nullptr;
    int Line = 0;
    const char* Function = nullptr;
    std::chrono::system_clock::time_point Time;
    std::thread::id ThreadId;
    std::shared_ptr<std::string> PreformattedMessage;
    std::string UnformattedMessage() const;

  protected:
    std::ostringstream m_MessageBuffer;
  };

} // namespace MKLogging
