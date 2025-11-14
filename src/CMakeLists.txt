target_sources(${PROJECT_NAME} PRIVATE
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

if(WIN32)
  target_sources(${PROJECT_NAME} PRIVATE "LogDebugOutputSink.cpp")
endif()


