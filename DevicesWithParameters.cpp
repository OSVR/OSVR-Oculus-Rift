/** @file
    @brief Implementation

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
#include "DevicesWithParameters.h"
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>

// Library/third-party includes
#include <json/reader.h>
#include <json/value.h>

#include <vrpn_Tracker_OculusRift.h>

// Standard includes
#include <iostream>

void createOculusRift(VRPNMultiserverData &data, OSVR_PluginRegContext ctx, const char *params) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(params, root)) {
        std::cerr << "-- createOculusRift(): ERROR: Could not parse configuration: " << reader.getFormattedErrorMessages() << std::endl;
        throw std::runtime_error("Could not parse configuration: " + reader.getFormattedErrorMessages());
    }

    int hmd_index = 0;
    try {
        hmd_index = root.get("index", 0).asInt();
    } catch (const std::exception& e) {
        std::cerr << "Caught exception HMD index from config file: " << e.what() << std::endl;
    }

    osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);

    reg.registerDevice(new vrpn_Tracker_OculusRift(reg.useDecoratedName(data.getName("OculusRift")).c_str(), reg.getVRPNConnection(), hmd_index));
}

