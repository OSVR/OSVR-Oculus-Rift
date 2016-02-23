#!/bin/bash

# This script installs Oculus SDK on the Travis CI Linux server.

# Parameters:
#  $1  Installation prefix

set -o errexit # exit on first error
set -o nounset # report unset variables
#set +o xtrace  # show commands

export OVR_VERSION=0.5.0.1

if [ $# -ne 1 ]; then
    echo "Usage: $0 <installation prefix>"
    exit 1
fi

export PREFIX="$1"
mkdir -p "${PREFIX}"

# If already installed, skip the rest
if [ -e "${PREFIX}/lib/libOVR.a" ]; then
    echo "OSVR is already installed."
    exit 0
fi

# Dependency source directories
mkdir -p ~/source/"${CONFIG}"
pushd ~/source/"${CONFIG}"

# Build Oculus SDK
curl -LR https://static.oculus.com/sdk-downloads/ovr_sdk_linux_${OVR_VERSION}.tar.xz -o ovr_sdk_linux_${OVR_VERSION}.tar.xz
tar xf ovr_sdk_linux_${OVR_VERSION}.tar.xz
mv ovr_sdk_linux_${OVR_VERSION} ovr_sdk
pushd ovr_sdk
find .
#make PREFIX="${PREFIX}" CC="${CC}" CXX="${CXX}" release install -j2
make PREFIX="${PREFIX}" install -j2
popd

popd

