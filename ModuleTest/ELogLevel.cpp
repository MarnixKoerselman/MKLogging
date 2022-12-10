module ELogLevel;
import ELogLevel;

void operator++(ELogLevel& logLevel)
{
  logLevel = static_cast<ELogLevel>(static_cast<int>(logLevel) + 1);
}

const char* ELogLevel_ToString(ELogLevel logLevel)
{
  switch (logLevel)
  {
  case ELogLevel::Verbose:    return "VERBOSE";
  case ELogLevel::Debug:      return "DEBUG";
  case ELogLevel::Info:       return "INFO";
  case ELogLevel::Warning:    return "WARNING";
  case ELogLevel::Error:      return "ERROR";
  case ELogLevel::None:       return "NONE";
  default:                    throw std::runtime_error("invalid log level");
  }
}

ELogLevel ELogLevel_FromString(const char* szLogLevel)
{
  for (ELogLevel logLevel = ELogLevel::Verbose; logLevel <= ELogLevel::None; ++logLevel)
  {
    if (_stricmp(szLogLevel, ELogLevel_ToString(logLevel)) == 0)
    {
      return logLevel;
    }
  }
  throw std::runtime_error(std::string("unrecognised log level: ") + szLogLevel);
}

std::ostream& operator<<(std::ostream& os, ELogLevel logLevel)
{
  return os << ELogLevel_ToString(logLevel);
}
