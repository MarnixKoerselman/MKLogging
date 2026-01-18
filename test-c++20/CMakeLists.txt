project(TestMKLogging-c++20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  TestSourceLocation.cpp
  "TestUtils.cpp" "TestUtils.h" "StringUtils.cpp" "StringUtils.h")

# C++20 tests use source_location on Linux, old API on Windows
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE MKL_USE_SOURCE_LOCATION=0)
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE MKL_USE_SOURCE_LOCATION=1)
endif()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  MKLogging
  gtest_main)

# add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
