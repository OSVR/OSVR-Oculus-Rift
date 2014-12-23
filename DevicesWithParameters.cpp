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
    std::cout << "-- createOculusRift() called." << std::endl;
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(params, root)) {
        throw std::runtime_error("Could not parse configuration: " +
                                 reader.getFormattedErrorMessages());
    }
    int hmd_index = root["index", 0].asInt();
    std::string hmd_type = root["type", "dk2"].asString();

    std::cout << "-- createOculusRift(): Registering OculusRift device with server." << std::endl;
    osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);

    reg.registerDevice(new vrpn_Tracker_OculusRift(reg.useDecoratedName(data.getName("OculusRift")).c_str(), reg.getVRPNConnection(), hmd_index, hmd_type.c_str()));
}
