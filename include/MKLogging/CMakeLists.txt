target_sources(${PROJECT_NAME} PRIVATE
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

if(WIN32)
  add_subdirectory("Windows")
endif()
