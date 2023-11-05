#pragma once

#include <filesystem>

namespace FileSystemUtils
{
  void CreateDirectoriesFromFilePath(const std::filesystem::path& filePath);
};

