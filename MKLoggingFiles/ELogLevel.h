#pragma once

enum class ELogLevel
{
    Verbose,  ///< Verbose debug logging.
    Debug,    ///< To be used while debugging.
    Info,     ///< Track progress of the application.
    Warning,  ///< Only warnings and more severe logging is written.
    Error,    ///< Only errors are logged.
    None,     ///< No logging.

    Min = Verbose,
    Max = None
};

const wchar_t* ELogLevel_ToString(ELogLevel logLevel) noexcept;
ELogLevel ELogLevel_FromString(const wchar_t* szLogLevel);

void operator++(ELogLevel& eValue);
