#pragma once

#include "ILogSink.h"
#include "ELogLevel.h"
#include <sstream>
#include <chrono>
#include <thread>
#include <memory>

struct LogRecord
{
  LogRecord(ELogLevel logLevel, const char* szFunction, const char* szFile, long lineNumber);
  LogRecord(const LogRecord& rhs);
  virtual ~LogRecord() = default;

  std::ostream& Get();
  void LogHex(const char* dataHeader, const void* data, int dataSize, int maxTraceValueCount = 128);
  std::string GetLogMessage() const;

  friend bool operator ==(const LogRecord& lhs, const LogRecord& rhs);

  ELogLevel LogLevel;
  const char* Function; // the function string appears to always be in a constant location (perhaps in the .text segment?) so no string copy required
  const char* File; // the file string appears to always be in a constant location (perhaps in the .text segment?) so no string copy required
  long LineNumber;
  std::chrono::system_clock::time_point Time;
  std::thread::id ThreadId;
  static uintptr_t ProcessId;
  std::shared_ptr<std::string> PreformattedMessage;

protected:
  std::ostringstream m_MessageBuffer;
};

