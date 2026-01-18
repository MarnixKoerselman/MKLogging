# MKLogging

- [MKLogging](#mklogging)
  - [Introduction](#introduction)
    - [Examples](#examples)
    - [Log Levels](#log-levels)
  - [Implementation Overview](#implementation-overview)
    - [Build System](#build-system)
    - [Core Types](#core-types)
    - [Logger](#logger)
      - [Configuration Options](#configuration-options)
    - [LogSink](#logsink)
      - [Creating Custom Log Sinks](#creating-custom-log-sinks)
    - [LogFormatter](#logformatter)
  - [Usage Considerations](#usage-considerations)
  - [Quick Start](#quick-start)
    - [Building the Library](#building-the-library)
    - [Windows / Linux](#windows--linux)
    - [Using in Your Project](#using-in-your-project)
    - [CMake Integration](#cmake-integration)
    - [Performance Notes](#performance-notes)
    - [Helpers](#helpers)
  - [Future Enhancements](#future-enhancements)
    - [Library Architecture Improvements](#library-architecture-improvements)
    - [CMake Build System Improvements](#cmake-build-system-improvements)

## Introduction

This is a simple logging framework, and its primary intended use is during development. There is nothing against using the same log facilities in production though.

To use the logger, a developer will typically use `LOGD`, `LOGI`, `LOGW` and `LOGE` macros to emit information. Typically this information ends up as `OutputDebugString` (in the Visual Studio output window), and in a log file (for post mortem analysis) during development, and in rotating log files when deployed in a production environment. Adding your own extensions that e.g. sends the log output to a central log server is totally possible and entirely up to you.

### Examples

```cpp
LOGD("i=" << i << ", x=" << x << ", y=" << y);
LOGE("failed to create file at " << filePath << " (error=" << errorCode << ")");
```

### Log Levels

- **Verbose** (`LOGV`)
- **Debug** (`LOGD`)
- **Information** (`LOGI`)
- **Warning** (`LOGW`)
- **Error** (`LOGE`)

## Implementation Overview

This is a cross-platform C++17 logging library built with CMake. It supports Windows (MSVC), Linux (GCC/Clang), and provides both 32-bit and 64-bit builds. The project uses modern CMake practices with presets for different configurations and includes comprehensive CI/CD pipelines for automated testing.

### Build System

- **CMake 3.19+** with presets for easy configuration
- **Cross-platform**: Windows (Visual Studio 2022), Linux (GCC/Clang)
- **Multi-architecture**: x86 and x64 support
- **Build configurations**: Debug, Release, RelWithDebInfo
- **Testing**: GoogleTest integration with CTest
- **CI/CD**: GitHub Actions for automated builds and testing

### Core Types

- **LogRecord** - Captures the information that makes up a log statement. It is unlikely that you'll interact directly with this type.
- **LogSink** - Writes the log statement to a specific output device, e.g. on the screen or to a file.
- **Logger** - Takes care of filtering and distributing log records to the connected sinks.
- **LogFormatter** - Takes care of the presentation of the metadata of log statements.

### Logger

There is a special Logger instance, which is accessed using the `LogCentral()` function. You can also create your own Logger instances, and configure them for a specific purpose.

#### Configuration Options

- **Log level threshold** - Only log items at or above this log level will be pushed to the log sinks
- **Log sinks** - Output destinations for log messages
- **Log formatter** - Controls the presentation of log metadata

For unit testing it's often useful to use a locally defined logger rather than `LogCentral()`, and use the extended version of the LOG macros (`MKL_LOGD`, `MKL_LOGI` etc).

A Logger instance has remarkable similarities with LogSink and is in fact derived from `ILogSink`. This means that Logger instances can be chained: you could e.g. create a secondary Logger instance that has log level threshold of `ELogLevel::Error`, and add it as listener to `LogCentral`. If `LogCentral` is configured to broadcast all log levels to its listeners, the secondary logger will only forward errors to its listeners (sinks).

### LogSink

Currently available log sinks are:

- **LogDebugOutputSink** - `OutputDebugString` (Windows only!)
- **LogOutputStreamSink** - `std::ostream` (user-provided reference)
- **LogStdOutSink** - `std::cout`
- **LogStdErrSink** - `std::cerr`
- **LogFileSink** - To a text file
- **LogUnbufferedFileSink** - To a text file, without buffer (less performance, better usability to investigate crashes)
- **LogRotatingFileSink** - Multiple text files, to limit the total size of logs
- **LogQueue** - A wrapper that allows each of the aforementioned log sinks to become buffered and asynchronous (NB: not recommended for LogDebugOutputSink)

#### Creating Custom Log Sinks

To create your own custom log sink, simply derive a class from `ILogSink` and implement its only method `OutputRecord`. Instantiate and use your class like this:

```cpp
using namespace MKLogging;

class MyLogSink : public ILogSink {
public:
    void OutputRecord(const LogRecord& record) override {
        // Your custom logging implementation
    }
};

// Add to central logger
LogCentral()->AddListener(std::make_shared<MyLogSink>());
```

### LogFormatter

Loggers have a default formatter, and this logs all metadata (source file, line number, function, thread & process ID) along with the user-provided log message. Log sinks do not have a default formatter, but it is possible to use a formatter on a sink. Please take note that formatting may be 'expensive' and formatters should be null-ed if possible.

Take a look at the implementation of the default formatter for ideas, and rewrite it or create a derived class with your own preferred formatting.

## Usage Considerations

A typical log command (e.g. `LOGD("var=" << var)`) has the following properties:

- **Performance** - Check if the log statement should be logged; if the minimum log level (property of LogCentral) is higher than - in this example - DEBUG, then the rest of the log statement is not evaluated, which means (almost) zero overhead for log statements that are ignored
- **Runtime Control** - The idea is that all log statements could stay in, even when these do not seem very interesting; but they can be enabled even at runtime, if your application enables that
- **Compile-time Removal** - If you wish, it is possible to completely discard log statements (e.g. if the release build should never ever contain logs at DEBUG level), by defining these macros before including Logger.h:

  ```cpp
  #define LOGD(x) (void)0
  #include "Logger.h"
  ```

- **UTF-8 Support** - The log statement is `std::ostream` based, for UTF-8. Conversion helpers for `std::wstring` and `wchar_t*` are provided, but they use `std::wstring_convert<std::codecvt_utf8<wchar_t>>` converter which is deprecated. The conversion helpers can be replaced (see Logger.h: `MKL_NO_STD_STRING_HELPERS`).

- **_Source location_** - in c++20 a new type `std::source_location` was included, which does not offer much under _MSVC_ (IMHO it makes the function name a little less readable) but the function name improvement for _GLIBCXX_ is significant. By default the c++20 implementation is used if possible, but this behaviour can be modified with compiler flag `MKL_USE_SOURCE_LOCATION` (see the `CMakeLists.txt` files in the test directories for examples).

## Quick Start

### Building the Library

```bash
# Configure with preset
cmake --preset linux-release  # or windows-x64-vs2022

# Build
cmake --build build/linux-release --parallel

# Run tests
ctest --test-dir build/linux-release
```

### Windows / Linux

There is a devcontainer configuration, which is what I use myself, mostly. If you're on a Windows machine and have Docker installed (e.g. VS2022 or later with Docker Desktop, or VSCode with any Docker) then you can simply switch to the devcontainer to start developing in a Linux environment.

_Gotcha: I'm not sure if this is typical for Rancher Desktop, but often I have a problem switching to the container, something about a mount point that is not configured correctly. It turns out that there are file under `\\wsl.localhost\Ubuntu\mnt\wslg\runtime-dir` (`wayland-0` and `wayland-0.lock`) that need to be removed or renamed. Paste this directory path in Windows Explorer to see the magic of WSL integration in action, and delete these files if necessary._

### Using in Your Project

```cpp
#include <MKLogging/Logger.h>
#include <MKLogging/LogFileSink.h>

using namespace MKLogging;

int main() {
    // Configure log level
    LogCentral()->SetMinimumLogLevel(ELogLevel::Info);

    // Add file logging
    LogCentral()->AddListener(std::make_shared<LogFileSink>("app.log"));

    // Basic usage with central logger
    LOGI("Application started");
    LOGD("Debug info: value=" << 42);
    LOGW("Warning message");
    LOGE("Error occurred");

    return 0;
}
```

### CMake Integration

Example using find_package:

```cmake
find_package(MKLogging REQUIRED)
target_link_libraries(your_target MKLogging::MKLogging)
```

Example using the [`FetchContent` CMake module](https://cmake.org/cmake/help/latest/module/FetchContent.html):

```cmake
cmake_minimum_required(VERSION 3.19)
project(your_target)

include(FetchContent)
FetchContent_Declare(
  mklogging
  GIT_REPOSITORY https://github.com/MarnixKoerselman/MKLogging.git
  GIT_TAG main   # or a specific git commit
)
FetchContent_MakeAvailable(mklogging)

add_executable(your_target)
target_link_libraries(
  your_target
  MKLogging
)
```

### Performance Notes

`iostream` based output is slower than `std::printf` based output. See [this Stack Overflow discussion](https://stackoverflow.com/questions/2872543/printf-vs-cout-in-c/20238349#20238349) for some thoughts.

Some tests (in `test/PerformanceTest.cpp`) demonstrate this difference.

For creating the log statement however, there are many advantages of using string stream buffer to construct the log statement: mostly ease-of-use (e.g. consistent logging of a class), and type safety.

### Helpers

Log when a method is entered and left:

```cpp
#include <LogEnterLeave.h>

using namespace MKLogging;

void f()
{
  LOG_ENTER_LEAVE(ELogLevel::Verbose);

  // do stuff...
}
```

## Future Enhancements

### Library Architecture Improvements

**Modern C++ Features**

- Upgrade to C++20 consistently (currently mixed C++17/20)
- Add `std::format` support as alternative to iostream for better performance
- Consider `std::span` for binary data logging instead of raw pointers

**Thread Safety & Performance**

- Add thread-safe logger variants with proper synchronization
- Implement lock-free logging for high-performance scenarios
- Add structured logging support (JSON/key-value pairs)
- Consider adding compile-time log level filtering

**API Modernization**

- Replace deprecated `std::codecvt` with modern UTF-8 conversion
- Add RAII-based scoped logging contexts
- Implement proper move semantics throughout
- Add `std::string_view` overloads where appropriate

### CMake Build System Improvements

**Package Management**

- Add proper CMake package config with version compatibility
- Install targets and export configurations
- Use `find_dependency()` in config files
- Add vcpkg support with `vcpkg.json`
- Consider Conan integration

**Build Optimizations**

- Enable precompiled headers for faster builds
- Add sanitizer presets (AddressSanitizer, ThreadSanitizer)
- Implement proper install rules with `GNUInstallDirs`
- Add static analysis integration (clang-tidy, cppcheck)

**Cross-Platform Improvements**

- Add macOS support to CI/presets
- Improve Android/iOS build support
- Add proper RPATH handling for shared libraries
- Consider adding shared library option

**Development Experience**

- Add clang-format configuration
- Include pre-commit hooks setup
- Add benchmark targets for performance testing
- Implement proper documentation generation with Doxygen
