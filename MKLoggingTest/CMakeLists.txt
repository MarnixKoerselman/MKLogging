cmake_minimum_required(VERSION 3.14)

add_executable(${TEST_EXECUTABLE_NAME}
  "FakeFileLogSink.cpp"
  "FakeFileLogSink.h"
  "FakeStringLogSink.h"
  "main.cpp"
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
)
target_link_libraries(${TEST_EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME} gmock_main)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
