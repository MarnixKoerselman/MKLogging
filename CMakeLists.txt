cmake_minimum_required(VERSION 3.19)

project(MKLogging)

if(MSVC)
  add_compile_options(/MP)
endif()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(LIBRARY_NAME MKLogging)
set(TEST_EXECUTABLE_NAME MKLoggingTest)

include(FetchContent)

FetchContent_Declare(
  GSL
  GIT_REPOSITORY https://github.com/microsoft/GSL
  GIT_TAG        v4.0.0
  GIT_SHALLOW    ON
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
  GIT_SHALLOW    ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF)

# Projects should aim to declare the details of all dependencies they might use before they call FetchContent_MakeAvailable() for any of them. This ensures that if any of the dependencies are also sub-dependencies of one or more of the others, the main project still controls the details that will be used (because it will declare them first before the dependencies get a chance to).
FetchContent_MakeAvailable(googletest GSL)

# Enables testing for this directory and below.
# This command should be in the source directory root because ctest expects to find a test file in the build directory root.
enable_testing()

add_subdirectory(MKLoggingFiles)
add_subdirectory(MKLoggingTest)
