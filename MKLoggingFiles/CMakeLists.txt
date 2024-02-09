cmake_minimum_required(VERSION 3.14)

add_library(${LIBRARY_NAME} STATIC
  "ELogLevel.cpp"
  "ELogLevel.h"
  "EventSource.h"
  "FileSystemUtils.cpp"
  "FileSystemUtils.h"
  "ILogSink.h"
  "LogEnterLeave.cpp"
  "LogEnterLeave.h"
  "LogFileSink.cpp"
  "LogFileSink.h"
  "LogFormatter.cpp"
  "LogFormatter.h"
  "Logger.cpp"
  "Logger.h"
  "LogOutputStreamSink.cpp"
  "LogOutputStreamSink.h"
  "LogQueue.cpp"
  "LogQueue.h"
  "LogRecord.cpp"
  "LogRecord.h"
  "LogRecordAutoSink.cpp"
  "LogRecordAutoSink.h"
  "LogRotatingFileSink.cpp"
  "LogRotatingFileSink.h"
  "LogSinkWithFormatter.cpp"
  "LogSinkWithFormatter.h"
  "LogUnbufferedFileSink.cpp"
  "LogUnbufferedFileSink.h"
)
target_include_directories(${LIBRARY_NAME} PUBLIC "./")

IF (WIN32)
target_sources(${LIBRARY_NAME}
  PRIVATE "Windows\\LogDebugOutputSink.cpp"
  PRIVATE "Windows\\LogDebugOutputSink.h")
ENDIF()

target_link_libraries(${LIBRARY_NAME} PRIVATE Microsoft.GSL::GSL)
