#pragma once

// This is the main include file.

#include "LogCentral.h"

// This global variable must be defined once in every project that uses the logging facilities.
// Currently this is done through a global instance in the LogLib.
//extern std::shared_ptr<CLogCentral> g_pLogger;
extern CLogCentral g_LogCentral;

// Set of macros that simplify logging using the default global logger

#ifndef ISLOGGED
#define ISLOGGED(mp_LogLevel) MKL_ISLOGGED(g_LogCentral, mp_LogLevel)
#endif

#ifndef LOGV
#define LOGV() MKL_LOGV(g_LogCentral)
#endif

#ifndef LOGD
#define LOGD(mp_out) MKL_LOGD(g_LogCentral, mp_out)
#endif

#ifndef LOGI
#define LOGI(mp_out) MKL_LOGI(g_LogCentral, mp_out)
#endif

#ifndef LOGW
#define LOGW(mp_out) MKL_LOGW(g_LogCentral, mp_out)
#endif

#ifndef LOGE
#define LOGE(mp_out) MKL_LOGE(g_LogCentral, mp_out)
#endif

#ifndef LOGHEX
#define LOGHEX(mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize) MKL_LOGHEX(g_LogCentral, mp_LogLevel, mp_DataHeader, mp_Data, mp_DataSize)
#endif
