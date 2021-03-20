#pragma once

#include "ILogSink.h"
#include "EventSource.h"
#include "ELogLevel.h"
#include "LogStatement.h"

class CLogCentral : public CEventSource<ILogSink>, public ILogSink
{
public:
    CLogCentral(const std::initializer_list<std::shared_ptr<ILogSink>>& pListeners);

    void SetMinimumLogLevel(ELogLevel logLevel);
    bool IsLogged(ELogLevel logLevel) noexcept;

public: // ILogSink
    void OutputString(const std::wstring& text) override;

private:
    ELogLevel m_MinimumLogLevel;
};


// Set of macros that simplify logging with a CLogCentral instance.
#define ENDLINE L"\n"
#define MKL_ISLOGGED(mp_logger, mp_LogLevel) mp_logger.IsLogged(mp_LogLevel)
#define MKL_LOGV(mp_logger) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Verbose)) { CLogStatement(&mp_logger).Get(ELogLevel::Verbose, __FUNCTION__, __FILE__, __LINE__) << ENDLINE; } } while (0)
#define MKL_LOGD(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Debug)) { CLogStatement(&mp_logger).Get(ELogLevel::Debug, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGI(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Info)) { CLogStatement(&mp_logger).Get(ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGW(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Warning)) { CLogStatement(&mp_logger).Get(ELogLevel::Warning, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGE(mp_logger, mp_out) do { if (MKL_ISLOGGED(mp_logger, ELogLevel::Error)) { CLogStatement(&mp_logger).Get(ELogLevel::Error, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGHEX(mp_logger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_logger, mp_LogLevel)) { CLogStatement(&mp_logger).LogHex(mp_LogLevel, __FUNCTION__, __FILE__, __LINE__, mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
