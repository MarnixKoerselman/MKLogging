project(TestMKLogging-c++17)

# std::format requires C++20
set(CMAKE_CXX_STANDARD 20)

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

# These tests use macro-based source location
target_compile_definitions(${PROJECT_NAME} PRIVATE MKL_USE_SOURCE_LOCATION=0)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  "MKLogging"
  "gmock_main"
)
