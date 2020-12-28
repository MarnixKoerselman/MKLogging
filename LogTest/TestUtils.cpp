#include "TestUtils.h"
#include "StringUtils.h"

std::filesystem::path GetTestOutputDirectoryPath(const wchar_t* szTestCaseName)
{
    // transform the test case name into something that can be used to construct a directory path
    std::wstring testCaseName(szTestCaseName);

    // remove everything from _Test - not needed to create a good name
    auto pos = testCaseName.rfind(L"_Test");
    if (pos != std::wstring::npos)
        testCaseName.erase(pos);

    // turn _ into /
    std::replace(testCaseName.begin(), testCaseName.end(), L'_', std::filesystem::path::preferred_separator);

    // the output is a subdirectory of the 'current' directory
    return std::filesystem::current_path() / testCaseName;

    // NB: if an exception is thrown, let it pass - this will crash the test, leading to the most sensible problem detection.
    // Helpers are not allowed to go wrong, so it is not necessary to recover from partial failure!
}

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

void EnsureCleanOutputDirectory(std::filesystem::path directoryPath)
{
    std::filesystem::remove_all(directoryPath);
    //std::filesystem::create_directories(directoryPath);
}

DirectoryEntry::DirectoryEntry(std::wstring path, uintmax_t fileSize, std::filesystem::file_time_type lastModifiedTime)
    : Path(path)
    , FileSize(fileSize)
    , LastModifiedTime(lastModifiedTime)
{
}

bool DirectoryEntry::operator==(const DirectoryEntry& rhs) const
{
    return (Path == rhs.Path) && (FileSize == rhs.FileSize) && (LastModifiedTime == rhs.LastModifiedTime);
}
