cmake --preset windows-x64-vs2022 -DPROJECT_VERSION_PARAM=%1 && cpack --config build/windows-x64-vs2022/CPackSourceConfig.cmake
