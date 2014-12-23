/** @file
    @brief Implementation of the "multiserver" plugin that offers the stock VRPN
   devices.

    @date 2014

    @author
    Kevin M. Godby
    <kevin@godby.org>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include "VRPNMultiserver.h"
#include "DevicesWithParameters.h"
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>

// Library/third-party includes
#include "vrpn_Connection.h"
#include "vrpn_Tracker_OculusRift.h"

// Standard includes
// - none

OSVR_PLUGIN(org_opengoggles_bundled_Multiserver) {
    osvr::pluginkit::PluginContext context(ctx);

    VRPNMultiserverData &data = *context.registerObjectForDeletion(new VRPNMultiserverData);
    osvrRegisterDriverInstantiationCallback(ctx, "OculusRift", &wrappedConstructor<&createOculusRift>, &data);

    return OSVR_RETURN_SUCCESS;
}

