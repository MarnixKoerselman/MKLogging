#pragma once

#include <string>

std::wstring GetWindowsErrorMessage(unsigned long errorCode); // from: GetLastError
std::wstring GetLastWindowsErrorMessage();
