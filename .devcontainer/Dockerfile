FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04
# see https://github.com/devcontainers/images/tree/main/src/cpp/history for versions

# ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.23.5"
ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="4.1.2"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# Install additional packages for build optimization
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        gcc-multilib \
        g++-multilib \
        lcov \
    && rm -rf /var/lib/apt/lists/*
