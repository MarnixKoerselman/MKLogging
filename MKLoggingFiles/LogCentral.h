#pragma once

#include "ILogSink.h"
#include "EventSource.h"
#include "ELogLevel.h"
#include "LogStatement.h"

class CLogCentral : public CEventSource<ILogSink>, public ILogSink
{
public:
    explicit CLogCentral(const std::initializer_list<std::shared_ptr<ILogSink>>& listeners);

    void SetMinimumLogLevel(ELogLevel logLevel);
    bool IsLogged(ELogLevel logLevel) noexcept;

public: // ILogSink
    void OutputString(const std::string& text) override;

private:
    ELogLevel m_MinimumLogLevel;
};


// Set of macros that simplify logging with a CLogCentral instance.
#define ENDLINE "\n"
#define MKL_ISLOGGED(mp_pLogger, mp_LogLevel) ((mp_pLogger != nullptr) && (mp_pLogger)->IsLogged(mp_LogLevel))
#define MKL_LOGV(mp_pLogger) do { if (MKL_ISLOGGED(mp_pLogger, ELogLevel::Verbose)) { CLogStatement(mp_pLogger).Get(ELogLevel::Verbose, __FUNCTION__, __FILE__, __LINE__) << ENDLINE; } } while (0)
#define MKL_LOGD(mp_pLogger, mp_out) do { if (MKL_ISLOGGED(mp_pLogger, ELogLevel::Debug)) { CLogStatement(mp_pLogger).Get(ELogLevel::Debug, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGI(mp_pLogger, mp_out) do { if (MKL_ISLOGGED(mp_pLogger, ELogLevel::Info)) { CLogStatement(mp_pLogger).Get(ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGW(mp_pLogger, mp_out) do { if (MKL_ISLOGGED(mp_pLogger, ELogLevel::Warning)) { CLogStatement(mp_pLogger).Get(ELogLevel::Warning, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGE(mp_pLogger, mp_out) do { if (MKL_ISLOGGED(mp_pLogger, ELogLevel::Error)) { CLogStatement(mp_pLogger).Get(ELogLevel::Error, __FUNCTION__, __FILE__, __LINE__) << mp_out << ENDLINE; } } while (0)
#define MKL_LOGHEX(mp_pLogger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_pLogger, mp_LogLevel)) { CLogStatement(mp_pLogger).LogHex(mp_LogLevel, __FUNCTION__, __FILE__, __LINE__, mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
