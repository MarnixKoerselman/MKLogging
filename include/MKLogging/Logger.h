#pragma once

// This is the main include file.

///////////////////////////////////////////////////////////////////////////////////////////////////
// Set of macros that simplify logging using the central logger

#ifndef ISLOGGED
#define ISLOGGED(mp_LogLevel) MKL_ISLOGGED(LogCentral(), mp_LogLevel)
#endif

#ifndef LOGV
#define LOGV(mp_Out) MKL_LOGV(LogCentral(), mp_Out)
#endif

#ifndef LOGD
#define LOGD(mp_Out) MKL_LOGD(LogCentral(), mp_Out)
#endif

#ifndef LOGI
#define LOGI(mp_Out) MKL_LOGI(LogCentral(), mp_Out)
#endif

#ifndef LOGW
#define LOGW(mp_Out) MKL_LOGW(LogCentral(), mp_Out)
#endif

#ifndef LOGE
#define LOGE(mp_Out) MKL_LOGE(LogCentral(), mp_Out)
#endif

#ifndef LOGHEX
#define LOGHEX(mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) MKL_LOGHEX(LogCentral(), mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Set of macros that simplify logging with a Logger instance.

#define MKL_ENDLINE "\n"

#ifndef MKL_ISLOGGED
#define MKL_ISLOGGED(mp_Logger, mp_LogLevel) (mp_Logger)->IsLogged(mp_LogLevel)
#endif

#ifndef MKL_LOGV
#define MKL_LOGV(mp_Logger, mp_Out) do { if (MKL_ISLOGGED(mp_Logger, ELogLevel::Verbose)) { LogRecordAutoSink(mp_Logger, ELogLevel::Verbose, __FUNCTION__, __FILE__, __LINE__).Get() << mp_Out << MKL_ENDLINE; } } while (0)
#endif

#ifndef MKL_LOGD
#define MKL_LOGD(mp_Logger, mp_Out) do { if (MKL_ISLOGGED(mp_Logger, ELogLevel::Debug)) { LogRecordAutoSink(mp_Logger, ELogLevel::Debug, __FUNCTION__, __FILE__, __LINE__).Get() << mp_Out << MKL_ENDLINE; } } while (0)
#endif

#ifndef MKL_LOGI
#define MKL_LOGI(mp_Logger, mp_Out) do { if (MKL_ISLOGGED(mp_Logger, ELogLevel::Info)) { LogRecordAutoSink(mp_Logger, ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__).Get() << mp_Out << MKL_ENDLINE; } } while (0)
#endif

#ifndef MKL_LOGW
#define MKL_LOGW(mp_Logger, mp_Out) do { if (MKL_ISLOGGED(mp_Logger, ELogLevel::Warning)) { LogRecordAutoSink(mp_Logger, ELogLevel::Warning, __FUNCTION__, __FILE__, __LINE__).Get() << mp_Out << MKL_ENDLINE; } } while (0)
#endif

#ifndef MKL_LOGE
#define MKL_LOGE(mp_Logger, mp_Out) do { if (MKL_ISLOGGED(mp_Logger, ELogLevel::Error)) { LogRecordAutoSink(mp_Logger, ELogLevel::Error, __FUNCTION__, __FILE__, __LINE__).Get() << mp_Out << MKL_ENDLINE; } } while (0)
#endif

#ifndef MKL_LOGHEX
#define MKL_LOGHEX(mp_Logger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_Logger, mp_LogLevel)) { LogRecordAutoSink(mp_Logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__).LogHex(mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// The logger class

#include "LogSinkWithFormatter.h"
#include "EventSource.h"
#include "ELogLevel.h"
#include "LogRecordAutoSink.h"

class Logger : public EventSource<ILogSink>, public LogSinkWithFormatter
{
public:
  //Logger();
  Logger(std::initializer_list<std::shared_ptr<ILogSink>> sinks = {});

  void SetMinimumLogLevel(ELogLevel logLevel);
  bool IsLogged(ELogLevel logLevel);

public: // ILogSink
  void OutputRecord(const LogRecord& record) override;

private:
  ELogLevel m_MinimumLogLevel;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// LogCentral: the central Logger instance.
// This global variable is instantiated automatically the first time it is used.
std::shared_ptr<Logger> SharedLogCentral();
Logger* LogCentral();

#ifndef MKL_NO_STD_STRING_HELPERS
// Helper to simplify/enrich the logging of std::wstring variables
std::ostream& operator <<(std::ostream& os, const std::wstring& s);
std::ostream& operator <<(std::ostream& os, const wchar_t* sz);
#endif
