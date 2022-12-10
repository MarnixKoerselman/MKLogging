#pragma once

#include "LogHelpers.h"
import Logger;
import ILogSink;

//#define MKL_NO_STD_STRING_HELPERS
#ifndef MKL_NO_STD_STRING_HELPERS
#include <iostream>
std::ostream& operator <<(std::ostream& os, const wchar_t* sz);
std::ostream& operator <<(std::ostream& os, const std::wstring& s);

inline std::ostream& operator <<(std::ostream& os, const char8_t*& sz) {
  return os << reinterpret_cast<const char*>(sz);
}
inline std::ostream& operator <<(std::ostream& os, const std::u8string& s) {
  return os << reinterpret_cast<const char*>(s.c_str());
}
#endif // MKL_NO_STD_STRING_HELPERS
