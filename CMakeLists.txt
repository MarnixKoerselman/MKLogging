cmake_minimum_required(VERSION 3.19)

# Version can be provided by build script via -DSOURCE_DIST_PROJECT_VERSION=x.y.z
set("PROJECT_DESCRIPTION" "Open source C++ log library donated to the world by Marnix Koerselman")

project(MKLogging VERSION 0.0.0 LANGUAGES CXX)

# Determine whether this is a standalone project or included by other projects
set(MKLOGGING_STANDALONE_PROJECT OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MKLOGGING_STANDALONE_PROJECT ON)
endif()

# Establish the project options
option(MKLOGGING_ENABLE_WARNINGS "Add warnings to CMAKE_CXX_FLAGS" ${MKLOGGING_STANDALONE_PROJECT})
option(MKLOGGING_COMPILE_WARNING_AS_ERROR "Make it impossible to ignore compiler warnings" ${MKLOGGING_STANDALONE_PROJECT})
option(MKLOGGING_BUILD_TESTS "Set to ON to build tests" ${MKLOGGING_STANDALONE_PROJECT})

# Enable Unity build for faster compilation
set(CMAKE_UNITY_BUILD OFF)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 10)

# Enable parallel builds by default
set(CMAKE_BUILD_PARALLEL_LEVEL 0) # 0 = use all available cores

# Enable interprocedural optimization in Release mode
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)

if(IPO_SUPPORTED AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Set output directories for all targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 20) # MKKLogging requires at least c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CXX standard and other compile options for standalone project
if(MKLOGGING_STANDALONE_PROJECT)
  # disable extensions
  set(CMAKE_CXX_EXTENSIONS OFF)

  # speed up compilation
  if(MSVC)
    add_compile_options(/MP)
    add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING _CRT_SECURE_NO_WARNINGS)

    add_link_options(
      $<$<NOT:$<CONFIG:DEBUG>>:/INCREMENTAL:NO> # Disable incremental linking.
      $<$<NOT:$<CONFIG:DEBUG>>:/OPT:REF> # Remove unreferenced functions and data.
      $<$<NOT:$<CONFIG:DEBUG>>:/OPT:ICF> # Identical COMDAT folding.
      $<$<CONFIG:DEBUG>:/INCREMENTAL:NO> # Disable incremental linking.
      $<$<CONFIG:DEBUG>:/OPT:NOREF> # No unreferenced data elimination.
      $<$<CONFIG:DEBUG>:/OPT:NOICF> # No Identical COMDAT folding.
    )
  endif()
endif()

# Tell MSVC to set the correct value for __cplusplus (see https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus and https://gitlab.kitware.com/cmake/cmake/-/issues/18837)
if(MSVC)
  add_compile_options(/Zc:__cplusplus)
endif()

if(MKLOGGING_COMPILE_WARNING_AS_ERROR)
  set(CMAKE_COMPILE_WARNING_AS_ERROR ${MKLOGGING_COMPILE_WARNING_AS_ERROR})
endif()

# Library target
add_library(${PROJECT_NAME} STATIC)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(MKLOGGING_ENABLE_WARNINGS)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
      /W4 # Enable high warning level
      /WX # Treat warnings as errors

      # /permissive-  # Disable non-conforming code
      # /w14242       # Conversion from 'type1' to 'type1', possible loss of data
      # /w14254       # Operator': conversion from 'type1:field_bits' to 'type2:field_bits'
      # /w14263       # Function does not override any base class virtual member function
      # /w14265       # Class has virtual functions, but destructor is not virtual
      # /w14287       # Unsigned/negative constant mismatch
      # /we4289       # Loop control variable declared in the for-loop is used outside
      # /w14296       # Expression is always 'boolean_value'
      # /w14311       # Pointer truncation from 'type1' to 'type2'
      # /w14545       # Expression before comma evaluates to a function which is missing an argument list
      # /w14546       # Function call before comma missing argument list
      # /w14547       # Operator before comma has no effect
      # /w14549       # Operator before comma has no effect
      # /w14555       # Expression has no effect
      # /w14619       # Pragma warning has no warning number
      # /w14640       # Enable warning on thread un-safe static member initialization
      # /w14826       # Conversion from 'type1' to 'type_2' is sign-extended
      # /w14905       # Wide string literal cast to 'LPSTR'
      # /w14906       # String literal cast to 'LPWSTR'
      # /w14928       # Illegal copy-initialization
    )
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE
      -Wall # Enable all common warnings
      -Wextra # Enable extra warnings
      -Wpedantic # Issue warnings needed for strict compliance to C++
      -Werror # Treat warnings as errors
      -Wcast-align # Warn for potential performance problem casts
      -Wconversion # Warn for implicit conversions that may alter a value
      -Wunused # Warn about unused variables
      -Wshadow # Warn about shadowing variables
      -Wformat=2 # Warn about printf format strings
      -Wundef # Warn about undefined identifier is evaluated
      -Wnull-dereference # Warn about NULL pointer dereference
      -Wdouble-promotion # Warn about implicit double promotion
      -Woverloaded-virtual # Warn about overloaded virtual function names
      -Wno-unused-function # Don't warn about unused functions (can be useful for headers)
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      target_compile_options(${PROJECT_NAME} PRIVATE
        -Wlogical-op # Warn about logical operations being used where bitwise were probably wanted
        -Wuseless-cast # Warn about casting to the same type
        -Wzero-as-null-pointer-constant # Warn about using 0 as nullptr
      )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      target_compile_options(${PROJECT_NAME} PRIVATE
        -Wmost # Enable most warnings
        -Wextra-semi # Warn about redundant semicolons
        -Wno-c++98-compat # Don't warn about C++98 compatibility
      )
    endif()
  endif()
endif()

# Generate version header
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/MKLogging/Version.h"
  @ONLY
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
)

add_subdirectory("include")
add_subdirectory("src")

# Enable testing when requested by the user
if(MKLOGGING_BUILD_TESTS)
  # Enables testing for this directory and below.
  # This command should be in the source directory root because ctest expects to find a test file in the build directory root.
  enable_testing()

  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW ON
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF)
  FetchContent_MakeAvailable(googletest)

  add_subdirectory("test-c++17")
  add_subdirectory("test-c++20")
endif()

# CPack configuration - Source packages only
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${SOURCE_DIST_PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "Marnix Koerselman")
set(CPACK_PACKAGE_CONTACT "https://github.com/MarnixKoerselman/MKLogging")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_STRIP_FILES OFF)

# Default CPack action is source package generation
set(CPACK_GENERATOR "")
set(CPACK_SOURCE_GENERATOR "ZIP")

# Only build source package if SOURCE_DIST_PROJECT_VERSION is specified
if(SOURCE_DIST_PROJECT_VERSION)
  # Create CMakeLists.txt with hardcoded version for source distribution
  file(READ "${CMAKE_CURRENT_LIST_FILE}" ORIGINAL_CMAKELISTS)
  string(REPLACE
    "project(MKLogging VERSION 0.0.0 LANGUAGES CXX)"
    "project(MKLogging VERSION ${SOURCE_DIST_PROJECT_VERSION} LANGUAGES CXX)"
    VERSIONED_CMAKELISTS "${ORIGINAL_CMAKELISTS}")

  # Verify the replacement worked
  if(NOT VERSIONED_CMAKELISTS MATCHES "VERSION ${SOURCE_DIST_PROJECT_VERSION}")
    message(FATAL_ERROR "Version replacement failed in source package CMakeLists.txt")
  endif()

  # Copy distributable files to staging directory
  file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/root")
  file(WRITE "${CMAKE_SOURCE_DIR}/dist/root/CMakeLists.txt" "${VERSIONED_CMAKELISTS}")
  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_SOURCE_DIR}/README.md" "${CMAKE_CURRENT_SOURCE_DIR}/CMakePresets.json" DESTINATION "${CMAKE_SOURCE_DIR}/dist/root")

  set(CPACK_SOURCE_INSTALLED_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/dist/root" "."
    "${CMAKE_CURRENT_SOURCE_DIR}/include" "include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src" "src"
    "${CMAKE_CURRENT_SOURCE_DIR}/test" "test"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "cmake"
  )
endif()

include(CPack)
