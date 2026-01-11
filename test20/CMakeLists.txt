project(TestMKLogging-c++20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  TestSourceLocation.cpp
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  MKLogging
  gtest_main
)

# add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})