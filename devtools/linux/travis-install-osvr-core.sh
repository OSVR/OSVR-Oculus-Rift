#!/bin/bash

# This script installs OSVR-Core on the Travis CI Linux server.

# Parameters:
#  $1  Installation prefix

set -o errexit # exit on first error
set -o nounset # report unset variables
#set +o xtrace  # show commands

if [ $# -ne 1 ]; then
    echo "Usage: $0 <installation prefix>"
    exit 1
fi

export PREFIX="$1"
mkdir -p "${PREFIX}"

# Dependency source directories
mkdir -p ~/source/"${CONFIG}"
pushd ~/source/"${CONFIG}"

# Build OSVR-Core dependencies
./travis-install-jsoncpp.sh
./travis-install-libfunctionality.sh
./travis-install-opencv.sh

# Download and build OSVR-Core
if [ -d OSVR-Core ]; then
    pushd OSVR-Core
    git pull
    git submodule update --init --recursive
    popd
else
    git clone --recursive https://github.com/OSVR/OSVR-Core.git
fi

# Build OSVR-Core
mkdir -p OSVR-Core/build
pushd OSVR-Core/build
cmake .. -DCMAKE_INSTALL_PREFIX="${PREFIX}" -DCMAKE_BUILD_TYPE="${CONFIG}"
make -j2 install
popd

popd

