#pragma once

// std::format requires C++20
static_assert(__cplusplus >= 202002L || _MSVC_LANG >= 202002L, "MKLogging requires C++20 or later");

// Configuration for source location support
// This can be overridden by defining MKL_USE_SOURCE_LOCATION before including headers
// or by setting it as a compile definition in CMake
#if !defined(MKL_USE_SOURCE_LOCATION)
#if __cplusplus >= 202002L
#define MKL_USE_SOURCE_LOCATION 1
#else
#define MKL_USE_SOURCE_LOCATION 0
#endif
#endif
