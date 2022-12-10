module;
#include "Logger.h"

module FileSystemUtils;
import <filesystem>;

namespace FileSystemUtils
{
  void CreateDirectoriesFromFilePath(const std::wstring& filePath)
  {
    std::filesystem::path directoryPath = std::filesystem::path(filePath).parent_path();

    if (!directoryPath.empty() && !std::filesystem::exists(directoryPath))
    {
      std::error_code errorCode;
      if (!std::filesystem::create_directories(directoryPath, errorCode))
      {
        LOGE("Could not create directories " << directoryPath << " (" << errorCode << ")");
      }
    }
  }
}
