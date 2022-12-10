export module LogRotatingFileSink;
import LogSinkWithFormatter;
import <filesystem>;
import LogFileSink;

// Take care: only use this logger if it is used asynchronous, e.g. through LogQueue. Using this directly (i.e. using this logger in the main thread)
// will most likely end up in infinite recursiveness, with a stack overflow as the fatal result.
export class LogRotatingFileSink : public LogSinkWithFormatter
{
public:
  LogRotatingFileSink(const std::filesystem::path& logFileDirectoryPath, size_t logFileSizeThreshold = 10 * 1024 * 1024, size_t maxLogFileCount = 10);

  virtual ~LogRotatingFileSink() = default;

public: // ILogSink
  void OutputRecord(const LogRecord& record) override;

protected: // methods made protected instead of private to improve testability
  void RollOver();
  std::filesystem::path GetNextFileName() const;
  std::wstring GenerateFileName(const std::chrono::system_clock::time_point& tv = std::chrono::system_clock::now()) const;
  std::wstring GetLogFileNameRegex() const;

private:
  const std::filesystem::path m_LogFileDirectoryPath;
  const std::wstring m_LogFileName = L"app"; // The base name of the log file(s). The timestamp of file creation will be inserted in the name, and ".log" appended. NB: should not contain a '.' (because used in regex)
  const std::wstring m_LogFileExtension = L"log"; // NB: should not contain a '.' (because used in regex)
  LogFileSink m_LogFile;
  const size_t m_FileSizeThreshold;
  const size_t m_MaxFileCount;
};
