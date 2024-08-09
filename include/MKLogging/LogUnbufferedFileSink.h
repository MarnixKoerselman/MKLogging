#pragma once

#include "LogSinkWithFormatter.h"
#include <string>
#include <filesystem>

class LogUnbufferedFileSink : public LogSinkWithFormatter
{
public:
  LogUnbufferedFileSink() = default;
  virtual ~LogUnbufferedFileSink();

  bool Create(const std::filesystem::path& filePath);
  bool OpenToAppend(const std::filesystem::path& filePath);
  void Close();

  void WriteToFile(const std::string& text);
  void WriteToFile(const void* data, size_t size);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override;

private:
  FILE* m_File = nullptr;
};
