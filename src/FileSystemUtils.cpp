#include "MKLogging/FileSystemUtils.h"
#include <filesystem>
#include "MKLogging/Logger.h"

namespace MKLogging
{

  void FileSystemUtils::CreateDirectoriesFromFilePath(const std::filesystem::path& filePath)
  {
    std::filesystem::path directoryPath = filePath.parent_path();

    if (!directoryPath.empty() && !std::filesystem::exists(directoryPath))
    {
      std::error_code errorCode;
      if (!std::filesystem::create_directories(directoryPath, errorCode))
      {
        LOGE("Could not create directories " << directoryPath << " (" << errorCode << ")");
      }
    }
  }

} // namespace MKLogging
