#include "LogCentral.h"
#include "Logger.h"

CLogCentral::CLogCentral(const std::list<std::shared_ptr<ILogSink>>& pListeners)
    : CEventSource<ILogSink>(pListeners)
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

void CLogCentral::OutputString(const std::wstring& text)
{
    std::lock_guard<std::recursive_mutex> lock(m_AccessListeners);
    for (const auto& pListener : m_pListeners)
    {
        pListener->OutputString(text);
    }
}

bool CLogCentral::IsLogged(ELogLevel logLevel) noexcept
{
    // TODO: allow overruling the hard-coded log level by dynamic application properties, to manipulate the log level at runtime

    return (logLevel >= m_MinimumLogLevel);
}
