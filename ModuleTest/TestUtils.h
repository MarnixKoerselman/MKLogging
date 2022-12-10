#pragma once

#include <filesystem>

#define GTEST_INFO(mp_out) std::cerr << "\033[0;32m[   INFO   ] \033[0;0m" << mp_out;

// type definitions
struct DirectoryEntry
{
  std::wstring Path;
  uintmax_t FileSize;
  std::filesystem::file_time_type LastModifiedTime;

  DirectoryEntry(std::wstring path, uintmax_t fileSize, std::filesystem::file_time_type lastModifiedTime);
  bool operator==(const DirectoryEntry& rhs) const;
};

using DirectoryEntries = std::vector<DirectoryEntry>;

using DataBuffer = std::vector<std::byte>; // requires C++17

// methods

// create a directory path based on the current directory, and the name of the testcase function : __FUNCTIONW__
#define TEST_OUTPUT_DIRECTORY_PATH GetTestOutputDirectoryPath(__FUNCTIONW__)
std::filesystem::path GetTestOutputDirectoryPath(const wchar_t* szTestCaseName);

std::filesystem::path GetApplicationPath();

// get the contents (files) of the specified directory
DirectoryEntries GetDirectoryContents(const std::filesystem::path& directoryPath);

// erase and recreate the specified directory, so that the directory is empty
void EnsureCleanOutputDirectory(const std::filesystem::path& directoryPath);

std::string ReadLogFileAsBinary(const std::filesystem::path& logFilePath);

int CountOccurrence(const std::string& string, const std::string& subString);

