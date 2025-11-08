#!/usr/bin/env bash

set -e

CMAKE_VERSION=${1:-"none"}

if [ "${CMAKE_VERSION}" = "none" ]; then
    CMAKE_VERSION="4.1.2"
    echo "No CMake version specified, using latest stable version ${CMAKE_VERSION}"
fi

# Cleanup temporary directory and associated files when exiting the script.
cleanup() {
    EXIT_CODE=$?
    set +e
    if [[ -n "${TMP_DIR}" ]]; then
        echo "Executing cleanup of tmp files"
        rm -Rf "${TMP_DIR}"
    fi
    exit $EXIT_CODE
}
trap cleanup EXIT


echo "Installing CMake version ${CMAKE_VERSION}..."

# Install build dependencies in case we need to build from source
apt-get update
apt-get -y install build-essential libssl-dev

# Remove existing CMake installation
apt-get -y purge --auto-remove cmake
rm -f /usr/local/bin/cmake /usr/local/bin/ctest /usr/local/bin/cpack /usr/local/bin/ccmake
rm -rf /opt/cmake
mkdir -p /opt/cmake

architecture=$(dpkg --print-architecture)
case "${architecture}" in
    arm64)
        ARCH=aarch64 ;;
    amd64)
        ARCH=x86_64 ;;
    *)
        echo "Unsupported architecture ${architecture}."
        exit 1
        ;;
esac

TMP_DIR=$(mktemp -d -t cmake-XXXXXXXXXX)
echo "Using temporary directory: ${TMP_DIR}"
cd "${TMP_DIR}"

# First try the binary distribution
echo "Attempting to download binary distribution..."
CMAKE_BINARY_NAME="cmake-${CMAKE_VERSION}-linux-${ARCH}.sh"
if curl -sSLf "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_BINARY_NAME}" -O; then
    echo "Binary distribution found, verifying checksum..."
    if curl -sSLf "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-SHA-256.txt" -O; then
        sha256sum -c --ignore-missing "cmake-${CMAKE_VERSION}-SHA-256.txt"
        echo "Installing from binary distribution..."
        sh "${CMAKE_BINARY_NAME}" --prefix=/opt/cmake --skip-license
    else
        echo "Checksum file not found, proceeding without verification..."
        sh "${CMAKE_BINARY_NAME}" --prefix=/opt/cmake --skip-license
    fi
else
    echo "Binary distribution not found, attempting to build from source..."
    # Try to download and build from source
    CMAKE_SOURCE_NAME="cmake-${CMAKE_VERSION}"
    CMAKE_ARCHIVE_NAME="${CMAKE_SOURCE_NAME}.tar.gz"

    if curl -sSLf "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_ARCHIVE_NAME}" -O; then
        echo "Source archive found, extracting..."
        tar xzf "${CMAKE_ARCHIVE_NAME}"
        cd "${CMAKE_SOURCE_NAME}"

        echo "Configuring CMake build..."
        ./bootstrap --prefix=/opt/cmake -- -DCMAKE_BUILD_TYPE:STRING=Release

        echo "Building CMake... (this may take a while)"
        make -j$(nproc)

        echo "Installing CMake..."
        make install
    else
        echo "Error: Neither binary nor source distribution found for CMake ${CMAKE_VERSION}"
        exit 1
    fi
fi

# Create symbolic links for CMake executables
for tool in cmake ctest cpack ccmake; do
    if [ -f "/opt/cmake/bin/${tool}" ]; then
        ln -sf "/opt/cmake/bin/${tool}" "/usr/local/bin/${tool}"
        echo "Created symlink for ${tool}"
    else
        echo "Warning: ${tool} not found in installation"
    fi
done
