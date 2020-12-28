# MkLogging

This is a simple logging framework, and its primary intended use is during development. There is nothing against using the same log facilities in production though.

To use the logger, a developer will typically use LOGD, LOGI, LOGW and LOGE macros to emit information. Typically this information ends up as OutputDebugString (in the debugging environment), and in a log file (for post morten analysis) during development, and in rotating log files when deployed in a production environment. Adding your own extensions that sends the log output to a central log server is totally possible and entirely up to you.


# Implementation in a nutshell

This is Microsoft based, using standard C++17 as much as possible (maybe you can use it on Linux too, YMMV). I used VS2019 to create the solution and project files. There is a 'Shared Items Project' (LogShared) that contains the files for the Log Lib. The consuming project LogTest is a GoogleTest application, and by referencing LogShared it pulls in all files from LogShared and compiles them with the same compiler settings. I thought that was a pretty neat option, as it saves me the hassle of keeping compiler settings for multiple projects in sync.

There's a global g_Logger instance that can be configured during application startup to add the appropriate log sinks. Currently available log sinks are:
 - LogDebugOutputSink => OutputDebugString
 - LogConsole => std::wcout
 - LogUtf8FileSink => to a text file that starts with the UTF-8 BOM, indicating the contents are in UTF-8 encoding
 - LogRotatingFileSink => multiple (UTF-8 encoded) text files, to limit the total size of logs
 - LogQueue => a wrapper that allows each of the aforementioned log sinks to become buffered and asynchronous (NB: not recommended for DebugOutput)

 For unit testing it's often useful to use a locally defined logger rather than g_Logger, and use the "DEBUG" version of the LOG macros (DLOGD, DLOGI etc).

## Notes

IOStream based output is slower than std::printf based output.
See https://stackoverflow.com/questions/2872543/printf-vs-cout-in-c/20238349#20238349 for some thoughts.

Some tests (in LogLibTests > PerformanceTest) show that this is correct.

For creating the log statement however, there are many advantages of using string stream buffer to construct the log statement: mostly ease-of-use (e.g. consistent logging of a class), and type safety.

