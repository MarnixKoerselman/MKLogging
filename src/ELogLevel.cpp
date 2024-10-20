#include "MKLogging/ELogLevel.h"
#include <cstring>

namespace
{
  bool isEqualCharCaseInsensitive(char a, char b)
  {
    return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
  }

  bool isEqualCaseInsentitive(std::string_view lhs, std::string_view rhs)
  {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), isEqualCharCaseInsensitive);
  }
}

// prefix increment operator
void operator++(ELogLevel& logLevel)
{
  logLevel = static_cast<ELogLevel>(static_cast<int>(logLevel) + 1);
}

std::ostream& operator<<(std::ostream& os, ELogLevel logLevel)
{
  return os << ELogLevel_ToString(logLevel);
}

//ELogLevel& operator++(ELogLevel& logLevel)
//{
//    if (logLevel >= ELogLevel::Max) {
//        logLevel = ELogLevel::Min;
//    }
//    else {
//        logLevel = static_cast<ELogLevel>(static_cast<int>(logLevel) + 1);
//    }
//    return logLevel;
//}

// postfix increment operator
//ELogLevel operator++(ELogLevel& logLevel, int)
//{
//    ELogLevel eOldValue = logLevel;
//    ++logLevel;
//    return eOldValue;
//}

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
    if (isEqualCaseInsentitive(szLogLevel, ELogLevel_ToString(logLevel)))
    {
      return logLevel;
    }
  }
  throw std::runtime_error(std::string("unrecognised log level: ") + szLogLevel);
}
