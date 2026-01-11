#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <memory>
#if __cplusplus >= 202002L
#include <source_location>
#endif

namespace MKLogging
{

  struct LogRecord
  {
    LogRecord(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
#if __cplusplus >= 202002L
    LogRecord(ELogLevel logLevel, std::source_location location = std::source_location::current());
#endif
    LogRecord(const LogRecord& rhs);
    virtual ~LogRecord() = default;

    std::ostream& Get();
    void LogHex(const char* dataHeader, const void* data, int dataSize, int maxTraceValueCount = 128);
    std::string GetLogMessage() const;

    friend bool operator ==(const LogRecord& lhs, const LogRecord& rhs);

    ELogLevel LogLevel;
    std::string_view Function; // the function string appears to always be in a constant location (perhaps in the .text segment?) so no string copy required
    std::string_view File; // the file string appears to always be in a constant location (perhaps in the .text segment?) so no string copy required
    long LineNumber;
    std::chrono::system_clock::time_point Time;
    std::thread::id ThreadId;
    std::shared_ptr<std::string> PreformattedMessage;
    std::string UnformattedMessage() const;

  protected:
    std::ostringstream m_MessageBuffer;
  };

} // namespace MKLogging
