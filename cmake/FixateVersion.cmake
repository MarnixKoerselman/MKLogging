# Script to fixate version in source package by replacing git-based version with hardcoded value

# Project version configured by CMake
set(PROJECT_VERSION "@PROJECT_VERSION@")

# Read CMakeLists.txt
file(READ "${CMAKE_SOURCE_DIR}/CMakeLists.txt" CMAKELISTS_CONTENT)

# Replace git version detection with hardcoded version
string(REGEX REPLACE 
  "# Get version from git tag[^#]*# Use git version or fallback[^}]*}"
  "# Version fixated for source distribution\nset(PROJECT_VERSION_STRING \"${PROJECT_VERSION}\")"
  CMAKELISTS_CONTENT "${CMAKELISTS_CONTENT}")

# Write back to CMakeLists.txt
file(WRITE "${CMAKE_SOURCE_DIR}/CMakeLists.txt" "${CMAKELISTS_CONTENT}")

message(STATUS "Version fixated to ${PROJECT_VERSION} in source package")