#include "LogCentral.h"
#include "Logger.h"

CLogCentral::CLogCentral(const std::initializer_list<std::shared_ptr<ILogSink>>& listeners)
    : CEventSource<ILogSink>(listeners)
    , m_MinimumLogLevel(ELogLevel::Warning)
{
#ifdef _DEBUG
    m_MinimumLogLevel = ELogLevel::Verbose;
#endif
}

void CLogCentral::SetMinimumLogLevel(ELogLevel logLevel)
{
    LOGW(ELogLevel_ToString(logLevel));
    m_MinimumLogLevel = logLevel;
}

void CLogCentral::OutputString(const std::string& text)
{
    std::lock_guard<std::recursive_mutex> lock(m_AccessListeners);
    for (const auto& pListener : m_Listeners)
    {
        pListener->OutputString(text);
    }
}

bool CLogCentral::IsLogged(ELogLevel logLevel) noexcept
{
    // TODO: allow overruling the hard-coded log level by dynamic application properties, to manipulate the log level at runtime

    return (logLevel >= m_MinimumLogLevel) && HasListeners();
}
