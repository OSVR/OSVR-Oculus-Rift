#!/bin/bash

# This script installs Oculus SDK on the Travis CI OS X server.

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

# Dependency source directories
mkdir -p ~/source/"${CONFIG}"
pushd ~/source/"${CONFIG}"

# Download and build Oculus SDK
if [ -d ovr_sdk_osx_-${OVR_VERSION} ]; then
    echo "Oculus SDK $OVR_VERSION has already been installed."
    exit 0
fi

# Build Oculus SDK
mkdir -p ovr_sdk
pushd ovr_sdk
curl -LR https://static.oculus.com/sdk-downloads/ovr_sdk_macos_${OVR_VERSION}.tar.gz -o ovr_sdk_macos_${OVR_VERSION}.tar.gz
tar xf ovr_sdk_macos_${OVR_VERSION}.tar.gz
popd
pushd OculusSDK
#make PREFIX="${PREFIX}" release install
popd

popd

