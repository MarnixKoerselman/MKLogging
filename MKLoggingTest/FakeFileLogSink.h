#pragma once

#include "Logger.h"
#include <filesystem>
#include <fstream>

class TestFileStream : public ILogSink
{
public:
  TestFileStream(std::filesystem::path filePath);
  virtual ~TestFileStream();
  std::ostream& GetStream();

public: // ILogSink
  virtual void OutputRecord(const LogRecord& record) override;

private:
  std::ofstream m_LogFile;
};

class TestStdFile
{
public:
  TestStdFile(std::filesystem::path filePath);
  virtual ~TestStdFile();
  void PrintF(const wchar_t* szFormat, ...);

private:
  FILE* m_File;
};
