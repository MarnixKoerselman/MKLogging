#pragma once

#include <filesystem>

namespace MKLogging
{

  namespace FileSystemUtils
  {
    void CreateDirectoriesFromFilePath(const std::filesystem::path& filePath);
  };

} // namespace MKLogging
