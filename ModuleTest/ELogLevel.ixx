export module ELogLevel;
import <iostream>;

export enum class ELogLevel
{
  Verbose,  ///< Verbose debug logging.
  Debug,    ///< To be used while debugging.
  Info,     ///< Track progress of the application.
  Warning,  ///< Only warnings and more severe logging is written.
  Error,    ///< Only errors are logged.
  None,     ///< No logging.

  Min = Verbose,
  Max = None,
  All = Min,
};

export void operator++(ELogLevel& logLevel);

export const char* ELogLevel_ToString(ELogLevel logLevel);

export ELogLevel ELogLevel_FromString(const char* szLogLevel);

export std::ostream& operator<<(std::ostream& os, ELogLevel logLevel);
