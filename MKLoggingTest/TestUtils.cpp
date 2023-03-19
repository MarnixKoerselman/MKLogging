#include "TestUtils.h"
#include "StringUtils.h"
#include <gtest/gtest.h>
#include <Windows.h> // apparently there is no standard way to get the executable's path => GetApplicationPath()

DirectoryEntry::DirectoryEntry(std::wstring path, uintmax_t fileSize, std::filesystem::file_time_type lastModifiedTime)
  : Path(path)
  , FileSize(fileSize)
  , LastModifiedTime(lastModifiedTime)
{}

bool DirectoryEntry::operator==(const DirectoryEntry& rhs) const
{
  return (Path == rhs.Path) && (FileSize == rhs.FileSize) && (LastModifiedTime == rhs.LastModifiedTime);
}

std::filesystem::path GetTestOutputDirectoryPath(const wchar_t* szTestCaseName)
{
  // transform the test case name into something that can be used to construct a directory path
  std::wstring testCaseName(szTestCaseName);

  // remove everything from _Test - not needed to create a good name
  auto pos = testCaseName.rfind(L"_Test");
  if (pos != std::wstring::npos)
    testCaseName.erase(pos);

  // replace :: (when a test fixture is used)
  size_t offset = 0;
  while (offset != std::wstring::npos)
  {
    offset = testCaseName.find(L"::", offset);
    if (offset != std::wstring::npos)
    {
      testCaseName.replace(offset, 2, L"_");
      offset += 1;
    }
  }

  // turn _ into /
  // std::replace(testCaseName.begin(), testCaseName.end(), L'_', std::filesystem::path::preferred_separator);

  // the output is a subdirectory of the directory that holds the test executable
  return GetApplicationPath().replace_filename(testCaseName);

  // NB: if an exception is thrown, let it pass - this will crash the test, leading to the most sensible problem detection.
  // Helpers are not allowed to go wrong, so it is not necessary to recover from partial failure!
}

std::filesystem::path GetApplicationPath()
{
  bool fileNameComplete = false;
  size_t bufferSize = _MAX_PATH;
  std::wstring sFilePath;
  do
  {
    sFilePath.resize(bufferSize);
    DWORD length = ::GetModuleFileNameW(nullptr, sFilePath.data(), static_cast<DWORD>(sFilePath.size()));
    fileNameComplete = (GetLastError() != ERROR_INSUFFICIENT_BUFFER);
    if (fileNameComplete)
    {
      sFilePath.resize(length);
      sFilePath.shrink_to_fit();
    }
    else
    {
      bufferSize *= 2;
    }
  } while (!fileNameComplete);
  return sFilePath;
}

// get the contents (files) of the specified directory
DirectoryEntries GetDirectoryContents(const std::filesystem::path& directoryPath)
{
  std::error_code errorCode;
  if (std::filesystem::is_empty(directoryPath, errorCode) || errorCode)
  {
    return {};
  }

  DirectoryEntries entries;
  for (auto& p : std::filesystem::directory_iterator(directoryPath))
  {
    entries.push_back(DirectoryEntry(p.path(), p.file_size(), p.last_write_time()));
  }
  return entries;
}

// erase and recreate the specified directory, so that the directory is empty
void EnsureCleanOutputDirectory(const std::filesystem::path& directoryPath)
{
  std::filesystem::remove_all(directoryPath);
}

void ReadLogFileAsBinary(const std::filesystem::path& logFilePath, __out std::string& buffer)
{
  struct _stat statBuffer = {0};
  ASSERT_EQ(0, _wstat(logFilePath.c_str(), &statBuffer));
  if (statBuffer.st_size > 0)
  {
    FILE* fileHandle = _wfsopen(logFilePath.c_str(), L"rb", _SH_DENYNO);
    ASSERT_NE(nullptr, fileHandle);
    buffer.resize(statBuffer.st_size);
    ASSERT_EQ(1, fread(buffer.data(), buffer.size(), 1, fileHandle));
    fclose(fileHandle);
  }
}

int CountOccurrence(const std::string& string, const std::string& subString)
{
  size_t offset = 0, n = 0;
  int count = 0;
  while (n = string.find(subString, offset), n != std::wstring::npos)
  {
    count += 1;
    offset = n + subString.length();
  }
  return count;
}
