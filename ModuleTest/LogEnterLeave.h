#pragma once

#ifndef MKL_LOG_ENTER_LEAVE
#define MKL_LOG_ENTER_LEAVE(mp_Logger, mp_LogLevel) LogEnterLeave mgen_LogEnterLeave(mp_Logger, mp_LogLevel, __func__, __FILE__, __LINE__);
#endif // !MKL_LOG_ENTER_LEAVE

#ifndef LOG_ENTER_LEAVE
#define LOG_ENTER_LEAVE(mp_LogLevel) MKL_LOG_ENTER_LEAVE(LogCentral(), mp_LogLevel)
#endif // !LOG_ENTER_LEAVE

import LogEnterLeave;
