#include "LogFileSink.h"
#include "Logger.h"
#include "FileSystemUtils.h"

CLogFileSink::~CLogFileSink()
{
	Close();
}

bool CLogFileSink::Create(const std::wstring& filePath)
{
	LOGD(L"filePath=" << filePath.c_str());

	if (m_File != nullptr)
	{
		Close();
	}

	// create the output directory if necessary
	FileSystemUtils::CreateDirectoriesFromFilePath(filePath);

	// Open in untranslated mode
	m_File = _wfsopen(filePath.c_str(), L"wb", _SH_DENYWR);
	const char* szUtf8Bom = "\xEF\xBB\xBF";
	fwrite(szUtf8Bom, 1, 3, m_File);
	m_FileSize = 3u;

	// https ://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fwrite
	// If file is opened with UTF-8 string processing, then only wstring values should be written
	// m_File = _wfsopen(filePath.c_str(), L"wt, ccs=UTF-8", _SH_DENYWR);

	return (m_File != nullptr);
}

void CLogFileSink::Close()
{
	if (m_File != nullptr)
	{
		const int result = fclose(m_File);
		m_File = nullptr;
		if (result != 0)
		{
			LOGE(L"fclose failed with error " << result);
		}
	}
}

size_t CLogFileSink::GetFileSize() const
{
	return m_FileSize;
}

bool CLogFileSink::IsOpen() const
{
	return (m_File != nullptr);
}

void CLogFileSink::WriteToFile(const std::string& text)
{
	if (IsOpen())
	{
		const size_t writtenElementCount = std::fwrite(text.data(), sizeof(std::string::value_type), text.size(), m_File);
		m_FileSize += writtenElementCount * sizeof(std::string::value_type);

		//const size_t writtenElementCount = std::fwrite(text.data(), sizeof(std::wstring::value_type), text.size(), m_File);
		//m_FileSize += writtenElementCount * sizeof(std::wstring::value_type);
	}
}

// ILogSink
void CLogFileSink::OutputRecord(const LogRecord& record)
{
	if (IsOpen())
	{
		std::stringstream buffer;
		OutputFormattedRecord(buffer, record);
		WriteToFile(buffer.str());
	}
}
