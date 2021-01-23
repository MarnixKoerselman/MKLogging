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

const wchar_t* ELogLevel_ToString(ELogLevel logLevel) noexcept
{
    switch (logLevel)
    {
        case ELogLevel::Verbose:    return L"VERBOSE";
        case ELogLevel::Debug:      return L"DEBUG";
        case ELogLevel::Info:       return L"INFO";
        case ELogLevel::Warning:    return L"WARNING";
        case ELogLevel::Error:      return L"ERROR";
        case ELogLevel::None:       return L"NONE";
        default:                    assert(false); return L"<invalid log level>";
    }
}

ELogLevel ELogLevel_FromString(const wchar_t* szLogLevel)
{
    for (ELogLevel value = ELogLevel::Verbose; value <= ELogLevel::None; ++value)
    {
        if (_wcsicmp(szLogLevel, ELogLevel_ToString(value)) == 0)
        {
            return value;
        }
    }
    assert(false);
    return ELogLevel::None;
}
