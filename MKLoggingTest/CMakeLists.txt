cmake_minimum_required(VERSION 3.14)

set(EXE_SOURCES
  "FakeFileLogSink.cpp"
  "main.cpp"
  "PerformanceTest.cpp"
  "StringUtils.cpp"
  "TestLogFileSink.cpp"
  "TestLogFormatters.cpp"
  "TestLogger.cpp"
  "TestLogHex.cpp"
  "TestLogQueue.cpp"
  "TestRotatingFileSink.cpp"
  "TestUnbufferedFileSink.cpp"
  "TestUtils.cpp"
)

set(EXE_HEADERS
  "FakeFileLogSink.h"
  "FakeStringLogSink.h"
  "MockLogSink.h"
  "StringUtils.h"
  "TestUtils.h"
)

add_executable(${TEST_EXECUTABLE_NAME} ${EXE_SOURCES} ${EXE_HEADERS})
target_link_libraries(${TEST_EXECUTABLE_NAME} PUBLIC ${LIBRARY_NAME} gmock_main)

gtest_discover_tests(${TEST_EXECUTABLE_NAME})
