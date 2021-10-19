#include "ELogLevel.h"
#include <cassert>
#include <cstring>

// prefix increment operator
void operator++(ELogLevel& eValue)
{
	eValue = static_cast<ELogLevel>(static_cast<int>(eValue) + 1);
}
//ELogLevel& operator++(ELogLevel& eValue)
//{
//    if (eValue >= ELogLevel::Max) {
//        eValue = ELogLevel::Min;
//    }
//    else {
//        eValue = static_cast<ELogLevel>(static_cast<int>(eValue) + 1);
//    }
//    return eValue;
//}

// postfix increment operator
//ELogLevel operator++(ELogLevel& eValue, int)
//{
//    ELogLevel eOldValue = eValue;
//    ++eValue;
//    return eOldValue;
//}

const char* ELogLevel_ToString(ELogLevel logLevel) noexcept
{
	switch (logLevel)
	{
		case ELogLevel::Verbose:    return "VERBOSE";
		case ELogLevel::Debug:      return "DEBUG";
		case ELogLevel::Info:       return "INFO";
		case ELogLevel::Warning:    return "WARNING";
		case ELogLevel::Error:      return "ERROR";
		case ELogLevel::None:       return "NONE";
		default:                    assert(false); return "<invalid log level>";
	}
}

ELogLevel ELogLevel_FromString(const char* szLogLevel)
{
	for (ELogLevel value = ELogLevel::Verbose; value <= ELogLevel::None; ++value)
	{
		if (_stricmp(szLogLevel, ELogLevel_ToString(value)) == 0)
		{
			return value;
		}
	}
	assert(false);
	return ELogLevel::None;
}
