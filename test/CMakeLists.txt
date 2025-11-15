project(TestMKLogging)

add_executable(${PROJECT_NAME}
  "FakeFileLogSink.cpp"
  "FakeFileLogSink.h"
  "FakeStringLogSink.h"
  "MockLogSink.h"
  "PerformanceTest.cpp"
  "StringUtils.cpp"
  "StringUtils.h"
  "TestLogFileSink.cpp"
  "TestLogFormatters.cpp"
  "TestLogger.cpp"
  "TestLogHex.cpp"
  "TestLogQueue.cpp"
  "TestRotatingFileSink.cpp"
  "TestUnbufferedFileSink.cpp"
  "TestUtils.cpp"
  "TestUtils.h"
  "TestELogLevel.cpp"
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  "MKLogging"
  "gmock_main"
)
