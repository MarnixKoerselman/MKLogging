#pragma once

#include <filesystem>
#include <vector>
#include <string_view>

#define GTEST_INFO(mp_out) std::cerr << "\033[0;32m[   INFO   ] \033[0;0m" << mp_out;

// type definitions
struct DirectoryEntry
{
  std::filesystem::path Path;
  uintmax_t FileSize;
  std::filesystem::file_time_type LastModifiedTime;

  DirectoryEntry(std::filesystem::path path, uintmax_t fileSize, std::filesystem::file_time_type lastModifiedTime);
  bool operator==(const DirectoryEntry& rhs) const;
};

using DirectoryEntries = std::vector<DirectoryEntry>;

using DataBuffer = std::vector<std::byte>; // requires C++17

// methods

// create a directory path based on the current directory, and the name of the testcase function
std::filesystem::path GetTestOutputDirectoryPath(std::string_view testCaseNameRaw);

#ifdef WIN32
std::filesystem::path GetApplicationPath();
#endif
std::filesystem::path GetCurrentDirectoryPath();

// get the contents (files) of the specified directory
DirectoryEntries GetDirectoryContents(const std::filesystem::path& directoryPath);

// erase and recreate the specified directory, so that the directory is empty
void EnsureCleanOutputDirectory(const std::filesystem::path& directoryPath);

void ReadLogFileAsBinary(const std::filesystem::path& logFilePath, /*__out*/ std::string& fileContents);

int CountOccurrence(const std::string& string, const std::string& subString);
