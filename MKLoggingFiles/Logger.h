#pragma once

// This is the main include file.

#include "LogCentral.h"

// This global variable must be defined once in every project that uses the logging facilities.
// Currently this is done through a global instance defined in Logger.cpp.
CLogCentral& LogCentral();

// Set of macros that simplify logging using the default global logger

#ifndef ISLOGGED
#define ISLOGGED(mp_LogLevel) MKL_ISLOGGED(LogCentral(), mp_LogLevel)
#endif

#ifndef LOGV
#define LOGV() MKL_LOGV(LogCentral())
#endif

#ifndef LOGD
#define LOGD(mp_out) MKL_LOGD(LogCentral(), mp_out)
#endif

#ifndef LOGI
#define LOGI(mp_out) MKL_LOGI(LogCentral(), mp_out)
#endif

#ifndef LOGW
#define LOGW(mp_out) MKL_LOGW(LogCentral(), mp_out)
#endif

#ifndef LOGE
#define LOGE(mp_out) MKL_LOGE(LogCentral(), mp_out)
#endif

#ifndef LOGHEX
#define LOGHEX(mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) MKL_LOGHEX(LogCentral(), mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize)
#endif

#ifndef LOG_NO_STD_STRING_HELPERS
// Helper to simplify/enrich the logging of std::wstring variables
std::ostream& operator <<(std::ostream& os, const std::wstring& s);
std::ostream& operator <<(std::ostream& os, const wchar_t* sz);
#endif
