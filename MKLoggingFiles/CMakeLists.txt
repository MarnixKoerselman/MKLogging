set(LIBRARY_SOURCES
  "ELogLevel.cpp"
  "FileSystemUtils.cpp"
  "LogEnterLeave.cpp"
  "LogFileSink.cpp"
  "LogFormatter.cpp"
  "Logger.cpp"
  "LogOutputStreamSink.cpp"
  "LogQueue.cpp"
  "LogRecord.cpp"
  "LogRecordAutoSink.cpp"
  "LogRotatingFileSink.cpp"
  "LogSinkWithFormatter.cpp"
  "LogUnbufferedFileSink.cpp"
)
set(LIBRARY_HEADERS
  "ELogLevel.h"
  "EventSource.h"
  "FileSystemUtils.h"
  "ILogSink.h"
  "LogEnterLeave.h"
  "LogFileSink.h"
  "LogFormatter.h"
  "Logger.h"
  "LogOutputStreamSink.h"
  "LogQueue.h"
  "LogRecord.h"
  "LogRecordAutoSink.h"
  "LogRotatingFileSink.h"
  "LogSinkWithFormatter.h"
  "LogUnbufferedFileSink.h"
)

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SOURCES}
  ${LIBRARY_HEADERS}
)
target_include_directories(${LIBRARY_NAME} PUBLIC "./")

IF (WIN32)
target_sources(${LIBRARY_NAME}
  PRIVATE "Windows\\LogDebugOutputSink.cpp"
  PRIVATE "Windows\\LogDebugOutputSink.h")
ENDIF()

target_link_libraries(${LIBRARY_NAME} PRIVATE Microsoft.GSL::GSL)
