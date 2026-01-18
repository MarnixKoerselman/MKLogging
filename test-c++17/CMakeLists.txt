project(TestMKLogging-c++17)

# this needs to be c++17 because of the utf-8 strings
set(CMAKE_CXX_STANDARD 17)

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
  "VersionTest.cpp"
)

# C++17 tests use macro-based source location by default
target_compile_definitions(${PROJECT_NAME} PRIVATE MKL_USE_SOURCE_LOCATION=0)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  "MKLogging"
  "gmock_main"
)
