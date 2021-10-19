#pragma once

#include "ILogSink.h"
#include "EventSource.h"
#include "ELogLevel.h"
#include "LogRecord.h"

class CLogCentral : public CEventSource<ILogSink>, public LogSinkWithFormatter
{
public:
	CLogCentral();

	void SetMinimumLogLevel(ELogLevel logLevel);
	bool IsLogged(ELogLevel logLevel);

public: // ILogSink
	void OutputRecord(const LogRecord& record) override;

private:
	ELogLevel m_MinimumLogLevel;
};


// Set of macros that simplify logging with a CLogCentral instance.
#define ENDLINE "\n"
#define MKL_ISLOGGED(mp_logger, mp_LogLevel) mp_logger.IsLogged(mp_LogLevel)
#define MKL_LOGV(mp_logger) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Verbose)) { LogRecord(&mp_logger, ELogLevel::Verbose, __FUNCTION__, __FILE__, __LINE__).Get() << ENDLINE; } } while (0)
#define MKL_LOGD(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Debug)) { LogRecord(&mp_logger, ELogLevel::Debug, __FUNCTION__, __FILE__, __LINE__).Get() << mp_out << ENDLINE; } } while (0)
#define MKL_LOGI(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Info)) { LogRecord(&mp_logger, ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__).Get() << mp_out << ENDLINE; } } while (0)
#define MKL_LOGW(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Warning)) { LogRecord(&mp_logger, ELogLevel::Warning, __FUNCTION__, __FILE__, __LINE__).Get() << mp_out << ENDLINE; } } while (0)
#define MKL_LOGE(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Error)) { LogRecord(&mp_logger, ELogLevel::Error, __FUNCTION__, __FILE__, __LINE__).Get() << mp_out << ENDLINE; } } while (0)
#define MKL_LOGHEX(mp_logger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_logger, mp_LogLevel)) { LogRecord(&mp_logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__).LogHex(mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
