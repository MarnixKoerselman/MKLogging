#pragma once

#include "LogSinkWithFormatter.h"
#include <filesystem>

class LogFileSink : public LogSinkWithFormatter
{
public:
  LogFileSink() = default;
  virtual ~LogFileSink();
  bool Create(const std::filesystem::path& filePath);
  void Close();

  size_t GetFileSize() const;
  bool IsOpen() const;

  void WriteToFile(const std::string& text);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override;

private:
  FILE* m_File = nullptr;
  size_t m_FileSize = 0;
};
