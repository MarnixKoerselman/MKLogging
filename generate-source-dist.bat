cmake --preset ninja-msvc-x86-debug -DSOURCE_DIST_PROJECT_VERSION=%1 && cpack --config build/ninja-msvc-x86-debug/CPackSourceConfig.cmake
