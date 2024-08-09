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

include(FetchContent)

FetchContent_Declare(
  GSL
  GIT_REPOSITORY https://github.com/microsoft/GSL
  GIT_TAG        v4.0.0
  GIT_SHALLOW    ON
)

FetchContent_MakeAvailable(GSL)

target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
