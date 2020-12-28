#pragma once

#include <filesystem>

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

// methods

// create a directory path based on the current directory, and the name of the testcase function : __FUNCTIONW__
#define TEST_OUTPUT_DIRECTORY_PATH GetTestOutputDirectoryPath(__FUNCTIONW__)
std::filesystem::path GetTestOutputDirectoryPath(const wchar_t* szTestCaseName);

// get the contents (files) of the specified directory
DirectoryEntries GetDirectoryContents(const std::filesystem::path& directoryPath);

// erase and recreate the specified directory, so that the directory is empty
void EnsureCleanOutputDirectory(std::filesystem::path directoryPath);
