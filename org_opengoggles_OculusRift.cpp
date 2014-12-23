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
#include "hidapi/hidapi.h"
#include "vrpn_Connection.h"
#include "vrpn_Tracker_OculusRift.h"
#include <boost/noncopyable.hpp>
#include <json/reader.h>
#include <json/value.h>

// Standard includes
#include <iostream>

OSVR_PLUGIN(org_opengoggles_bundled_Multiserver) {
    std::cout << "-- Creating plugin context..." << std::endl;
    osvr::pluginkit::PluginContext context(ctx);

    std::cout << "-- Registering VRPN multiserver data..." << std::endl;
    VRPNMultiserverData &data = *context.registerObjectForDeletion(new VRPNMultiserverData);

    std::cout << "-- Registering driver instantiation callback..." << std::endl;
    osvrRegisterDriverInstantiationCallback(ctx, "OculusRift", &wrappedConstructor<&createOculusRift>, &data);

    return OSVR_RETURN_SUCCESS;
}

