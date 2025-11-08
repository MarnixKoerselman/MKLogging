FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

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
        ccache \
        ninja-build \
        distcc \
    && rm -rf /var/lib/apt/lists/*

# Configure ccache
ENV CCACHE_DIR=/ccache
ENV PATH="/usr/lib/ccache:$PATH"
RUN mkdir -p /ccache \
    && chown -R vscode:vscode /ccache
