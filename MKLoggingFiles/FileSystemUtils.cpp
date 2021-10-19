#include "FileSystemUtils.h"
#include <filesystem>
#include "Logger.h"

void FileSystemUtils::CreateDirectoriesFromFilePath(const std::wstring& filePath)
{
	std::filesystem::path directoryPath(filePath);
	directoryPath.remove_filename();

	if (!directoryPath.empty() && !std::filesystem::exists(directoryPath))
	{
		std::error_code errorCode;
		if (!std::filesystem::create_directories(directoryPath, errorCode))
		{
			LOGE("Could not create directories " << directoryPath << " (" << errorCode << ")");
		}
	}
}
