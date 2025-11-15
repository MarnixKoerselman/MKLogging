# MkLogging

## Introduction

This is a simple logging framework, and its primary intended use is during development. There is nothing against using the same log facilities in production though.

To use the logger, a developer will typically use LOGD, LOGI, LOGW and LOGE macros to emit information. Typically this information ends up as OutputDebugString (in the Visual Studio output window), and in a log file (for post mortem analysis) during development, and in rotating log files when deployed in a production environment. Adding your own extensions that e.g. sends the log output to a central log server is totally possible and entirely up to you.

Examples:

- `LOGD("i=" << i << ", x=" << x << ", y=" << y);`
- `LOGE("failed to create file at " << filePath << " (error=" << errorCode << ")");`

Log levels:

- Verbose (LOGV)
- Debug (LOGD)
- Information (LOGI)
- Warning (LOGW)
- Error (LOGE)

## Implementation in a nutshell

This is a cross-platform C++17 logging library built with CMake. It supports Windows (MSVC), Linux (GCC/Clang), and provides both 32-bit and 64-bit builds. The project uses modern CMake practices with presets for different configurations and includes comprehensive CI/CD pipelines for automated testing.

### Build System

- **CMake 3.19+** with presets for easy configuration
- **Cross-platform**: Windows (Visual Studio 2022), Linux (GCC/Clang)
- **Multi-architecture**: x86 and x64 support
- **Build configurations**: Debug, Release, RelWithDebInfo
- **Testing**: GoogleTest integration with CTest
- **CI/CD**: GitHub Actions for automated builds and testing

### Types

- LogRecord captures the information that makes up a log statement. It is unlikely that you'll interact directly with this type.
- LogSink writes the log statement to a specific output device, e.g. on the screen or to a file.
- Logger takes care of filtering and distributing log records to the connected sinks.
- LogFormatter takes care of the presentation of the metadata of log statements.

#### Logger

There is a special Logger instance, which is accessed using the `LogCentral()` function. You can also create your own Logger instances, and configure them for a specific purpose.

Configuration items are:

- the log level threshold (only log items at or above this log level will be pushed to the log sinks)
- the log sinks
- the log formatter

For unit testing it's often useful to use a locally defined logger rather than LogCentral(), and use the extended version of the LOG macros (MKL_LOGD, MKL_LOGI etc).

A Logger instance has remarkable similarities with LogSink and is in fact derived from ILogSink. All that means that Logger instances can be chained: you could e.g. create a secondary Logger instance that has log level threshold of ELogLevel::Error, and add it as listener to LogCentral. If LogCentral is configured to broadcast all log levels to its listeners, the secondary logger will only forward errors to its listeners (sinks).

#### LogSink

Currently available log sinks are:

- LogDebugOutputSink => OutputDebugString
- LogOutputStreamSink => std::ostream (user-provided reference)
- LogStdOutSink => std::cout
- LogStdErrSink => std::cerr
- LogFileSink => to a text file
- LogUnbufferedFileSink => to a text file, without buffer (less performance, better usability to investigate crashes)
- LogRotatingFileSink => multiple text files, to limit the total size of logs
- LogQueue => a wrapper that allows each of the aforementioned log sinks to become buffered and asynchronous (NB: not recommended for LogDebugOutputSink)

To create your own custom log sink, simply derive a class from ILogSink and implement its only method 'OutputRecord'. Instantiate and use your class like this:

```cpp
class MyLogSink : public ILogSink {
public:
    void OutputRecord(const LogRecord& record) override {
        // Your custom logging implementation
    }
};

// Add to central logger
LogCentral()->AddListener(std::make_shared<MyLogSink>());
```

#### LogFormatter

Loggers have a default formatter, and this logs all metadata (file, line number, function, thread & process ID) along with the user-provided log message. Log sinks do not have a default formatter, but it is possible to use a formatter on a sink. Please take note that formatting may be 'expensive' and formatters should be null-ed if possible.

Take a look at the implementation of the default formatter for ideas, and rewrite it or create a derived class with your own preferred formatting.

## Considerations for usage

A typical log command (e.g. `LOGD("var=" << var)`) has the following properties:

- check if the log statement should be logged; if the minimum log level (property of LogCentral) is higher than - in this example - DEBUG, then the rest of the log statement is not evaluated, which means (almost) zero overhead for log statements that are ignored
- the idea is that all log statements could stay in, even when these do not seem very interesting; but they can be enabled even at runtime, if your application enables that
- if you wish, it is possible to completely discard log statements (e.g. if the release build should never ever contain logs at DEBUG level), by defining these macros before including Logger.h:

  ```c++
  #define LOGD(x) (void)0
  #include "Logger.h"
  ```

- the log statement is std::ostream based, for UTF-8. Conversion helpers for std::wstring and (wchar_t*) are provided, but they use `std::wstring_convert<std::codecvt_utf8<wchar_t>>` converter which is deprecated. The conversion helpers can be replaced (see Logger.h: `MKL_NO_STD_STRING_HELPERS`).

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

### Using in Your Project

```cpp
#include <MKLogging/Logger.h>
#include <MKLogging/LogFileSink.h>

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

Example using the
[`FetchContent` CMake module](https://cmake.org/cmake/help/latest/module/FetchContent.html):

```cmake
cmake_minimum_required(VERSION 3.19)
project(your_target)

# MKLogging requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

IOStream based output is slower than std::printf based output.
See <https://stackoverflow.com/questions/2872543/printf-vs-cout-in-c/20238349#20238349> for some thoughts.

Some tests (in test/PerformanceTest.cpp) demonstrate this difference.

For creating the log statement however, there are many advantages of using string stream buffer to construct the log statement: mostly ease-of-use (e.g. consistent logging of a class), and type safety.

### Helpers

Log when a method is entered and left:

```c++
#include <LogEnterLeave.h>
void f()
{
  LOG_ENTER_LEAVE(ELogLevel::Verbose);

  // do stuff...
}
```
