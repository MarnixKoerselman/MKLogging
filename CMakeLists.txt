cmake_minimum_required(VERSION 3.19)

set("PROJECT_DESCRIPTION" "Open source C++ log library donated to the world by Marnix Koerselman")

project(MKLogging
  LANGUAGES CXX
)

# Determine whether this is a standalone project or included by other projects
set(MKLOGGING_STANDALONE_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(MKLOGGING_STANDALONE_PROJECT ON)
endif()

# Establish the project options
option(MKLOGGING_ENABLE_WARNINGS "Add warnings to CMAKE_CXX_FLAGS" ${MKLOGGING_STANDALONE_PROJECT})
option(MKLOGGING_COMPILE_WARNING_AS_ERROR "Make it impossible to ignore compiler warnings" ${MKLOGGING_STANDALONE_PROJECT})
option(MKLOGGING_BUILD_TESTS "Set to ON to build tests" ${MKLOGGING_STANDALONE_PROJECT})

# Set CXX standard and other compile options for standalone project
if (MKLOGGING_STANDALONE_PROJECT)
  # C++ standard
  if (MKLOGGING_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD ${MKLOGGING_CXX_STANDARD})
  else()
    set(CMAKE_CXX_STANDARD 17) # WARNING: library is currently incompatible with c++20
  endif()
  set(CMAKE_CXX_STANDARD_REQUIRED True)

  # disable extensions
  set(CMAKE_CXX_EXTENSIONS OFF)

  # speed up compilation
  if(MSVC)
    add_compile_options(/MP)
  endif()
endif()

if(MKLOGGING_ENABLE_WARNINGS)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    #   if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER_EQUAL 5.0)
    #     set(COMPILER_SPECIFIC_FLAGS "-Wsuggest-override")
    #   endif()
    # endif()

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wshadow -Weffc++ -Wsign-compare -Wshadow -Wwrite-strings -Wpointer-arith -Winit-self -Wconversion -Wno-sign-conversion ${COMPILER_SPECIFIC_FLAGS}")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wsign-compare -Wshadow -Wwrite-strings -Wpointer-arith -Winit-self -Wconversion -Wno-sign-conversion ${COMPILER_SPECIFIC_FLAGS}")
  endif()
endif()

if(MKLOGGING_COMPILE_WARNING_AS_ERROR)
  set(CMAKE_COMPILE_WARNING_AS_ERROR  ${MKLOGGING_COMPILE_WARNING_AS_ERROR})
endif()

add_library(${PROJECT_NAME} STATIC)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
add_subdirectory("include")
add_subdirectory("src")

# Enable testing when requested by the user
if(MKLOGGING_BUILD_TESTS)
  # Enables testing for this directory and below.
  # This command should be in the source directory root because ctest expects to find a test file in the build directory root.
  enable_testing()

  add_subdirectory("test")
endif()
