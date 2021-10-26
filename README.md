# MkLogging

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

# Implementation in a nutshell

This code is Microsoft based, using standard C++17 as much as possible (maybe you can use it on Linux too, YMMV). I used VS2019 to create the solution and project files. There is a 'Shared Items Project' (MKLoggingFiles) that contains the files for the library. The consuming project MKLoggingTest is a GoogleTest application, and by referencing LogShared it pulls in all files from MKLoggingFiles and compiles them with the same compiler settings. I thought that was a pretty neat option, as it saves me the hassle of keeping compiler settings for multiple projects in sync.

## Types
LogRecord captures the information that makes up a log statement. It is unlikely that you'll interact directly with this type.
LogSink writes the log statement to a specific output device, e.g. on the screen or to a file.
Logger takes care of filtering and distributing log records to the connected sinks.
LogFormatter takes care of the presentation of the metadata of log statements.

### Logger
There is a special Logger instance, which is accessed using the `LogCentral()` function. You can also create your own Logger instances, and configure them for a specific purpose.

Configuration items are:
- the log level threshold (only log items at or above this log level will be pushed to the log sinks)
- the log sinks
- the log formatter

For unit testing it's often useful to use a locally defined logger rather than LogCentral(), and use the extended version of the LOG macros (MKL_LOGD, MKL_LOGI etc).

A Logger instance has remarkable similarities with LogSink and is in fact derived from ILogSink. All that means that Logger instances can be chained: you could e.g. create a secondary Logger instance that has log level threshold of ELogLevel::Error, and add it as listener to LogCentral. If LogCentral is configured to broadcast all log levels to its listeners, the secondary logger will only forward errors to its listeners (sinks).

### LogSink
Currently available log sinks are:
 - LogDebugOutputSink => OutputDebugString
 - LogConsoleSink => std::wcout
 - LogFileSink => to a text file
 - LogRotatingFileSink => multiple text files, to limit the total size of logs
 - LogQueue => a wrapper that allows each of the aforementioned log sinks to become buffered and asynchronous (NB: not recommended for LogDebugOutputSink)

 To create your own custom long sink, simply derive a class from ILogSink and implement its only method 'OutputRecord'. Instantiate and use your class like this:

   `LogCentral()->AddListener(std::make_shared<MyLogSink>())`

### LogFormatter
Loggers have a default formatter, and this logs all metadata (file, line number, function, thread & process ID) along with the user-provided log message. Log sinks do not have a default formatter, but it is possible to use a formatter on a sink. Please take note that formatting may be 'expensive' and formatters should be null-ed if possible.

Take a look at the implementation of the default formatter for ideas, and rewrite it or create a derived class with your own preferred formatting.

# Considerations for usage

A typical log command (e.g. `LOGD("var=" << var)`) has the following properties:
- check if the log statement should be logged; if the minimum log level (property of LogCentral) is higher than - in this example - DEBUG, then the rest of the log statement is not evaluated, which means (almost) zero overhead for log statements that are ignored
- the idea is that all log statements could stay in, even when these do not seem very interesting; but they can be enabled even at runtime, if your application enables that
- if you wish, it is possible to completely discard log statements (e.g. if the release build should never ever contain logs at DEBUG level), by defining these macros before including Logger.h:
  ```
  #define LOGD(x) (void)0
  #include "Logger.h"
  ```
- the log statement is std::wostream based, to support UTF-x on Windows

## Notes

IOStream based output is slower than std::printf based output.
See https://stackoverflow.com/questions/2872543/printf-vs-cout-in-c/20238349#20238349 for some thoughts.

Some tests (in LogLibTests > PerformanceTest) show that this is correct.

For creating the log statement however, there are many advantages of using string stream buffer to construct the log statement: mostly ease-of-use (e.g. consistent logging of a class), and type safety.

## Helpers

Log when a method is entered and left:
```
#include <LogEnterLeave.h>
void f()
{
  LOG_ENTER_LEAVE(ELogLevel::Verbose);
}
```
