#pragma once

#include "Config.h"
#include <string_view>
#include <format>

// This is the main include file.

///////////////////////////////////////////////////////////////////////////////////////////////////
// Set of macros that simplify logging using the central logger

#ifndef ISLOGGED
#define ISLOGGED(mp_LogLevel) MKL_ISLOGGED(MKLogging::LogCentral(), mp_LogLevel)
#endif

#ifndef LOGV
#define LOGV(mp_Fmt, ...) MKL_LOGV(MKLogging::LogCentral(), mp_Fmt, ##__VA_ARGS__)
#endif

#ifndef LOGD
#define LOGD(mp_Fmt, ...) MKL_LOGD(MKLogging::LogCentral(), mp_Fmt, ##__VA_ARGS__)
#endif

#ifndef LOGI
#define LOGI(mp_Fmt, ...) MKL_LOGI(MKLogging::LogCentral(), mp_Fmt, ##__VA_ARGS__)
#endif

#ifndef LOGW
#define LOGW(mp_Fmt, ...) MKL_LOGW(MKLogging::LogCentral(), mp_Fmt, ##__VA_ARGS__)
#endif

#ifndef LOGE
#define LOGE(mp_Fmt, ...) MKL_LOGE(MKLogging::LogCentral(), mp_Fmt, ##__VA_ARGS__)
#endif

#ifndef LOGHEX
#define LOGHEX(mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) MKL_LOGHEX(MKLogging::LogCentral(), mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// Set of macros that simplify logging with a Logger instance.

#ifndef MKL_ISLOGGED
#define MKL_ISLOGGED(mp_Logger, mp_LogLevel) (mp_Logger)->IsLogged(mp_LogLevel)
#endif

#if MKL_USE_SOURCE_LOCATION

#include <source_location>

#ifndef MKL_LOGV
#define MKL_LOGV(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Verbose)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Verbose, std::source_location::current()); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGD
#define MKL_LOGD(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Debug)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Debug, std::source_location::current()); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGI
#define MKL_LOGI(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Info)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Info, std::source_location::current()); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGW
#define MKL_LOGW(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Warning)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Warning, std::source_location::current()); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGE
#define MKL_LOGE(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Error)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Error, std::source_location::current()); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGHEX
#define MKL_LOGHEX(mp_Logger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_Logger, mp_LogLevel)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, mp_LogLevel, std::source_location::current()); __rec.LogHex(mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
#endif

#else // !MKL_USE_SOURCE_LOCATION

#ifndef MKL_LOGV
#define MKL_LOGV(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Verbose)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Verbose, __FUNCTION__, __FILE__, __LINE__); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGD
#define MKL_LOGD(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Debug)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Debug, __FUNCTION__, __FILE__, __LINE__); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGI
#define MKL_LOGI(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Info)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGW
#define MKL_LOGW(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Warning)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Warning, __FUNCTION__, __FILE__, __LINE__); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGE
#define MKL_LOGE(mp_Logger, mp_Fmt, ...) do { if (MKL_ISLOGGED(mp_Logger, MKLogging::ELogLevel::Error)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, MKLogging::ELogLevel::Error, __FUNCTION__, __FILE__, __LINE__); __rec.SetMessage(std::format(mp_Fmt, ##__VA_ARGS__) + "\n"); } } while (0)
#endif

#ifndef MKL_LOGHEX
#define MKL_LOGHEX(mp_Logger, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) do { if (MKL_ISLOGGED(mp_Logger, mp_LogLevel)) { MKLogging::LogRecordAutoSink __rec(mp_Logger, mp_LogLevel, __FUNCTION__, __FILE__, __LINE__); __rec.LogHex(mp_DataHeader, mp_Data, mp_DataSize); } } while (0)
#endif

#endif // MKL_USE_SOURCE_LOCATION

#include "LogSinkWithFormatter.h"
#include "EventSource.h"
#include "ELogLevel.h"
#include "LogRecordAutoSink.h"

namespace MKLogging
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // The logger class

  class Logger : public EventSource<ILogSink>, public LogSinkWithFormatter
  {
  public:
    Logger(std::initializer_list<std::shared_ptr<ILogSink>> sinks = {});

    void SetMinimumLogLevel(ELogLevel logLevel);
    bool IsLogged(ELogLevel logLevel);

    template <typename LogSink>
    auto AddListener(const std::shared_ptr<LogSink>& pListener)
    {
      EventSource::AddListener(pListener);
      return pListener;
    }

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

  std::string_view GetVersion();

} // namespace MKLogging
