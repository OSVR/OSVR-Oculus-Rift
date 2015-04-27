/** @file
    @brief Implementation

    @date 2014

    @author
    Kevin M. Godby
    <kevin@godby.org>
*/

// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include "DevicesWithParameters.h"
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>
#include <osvr/Util/StringLiteralFileToString.h>

#include "com_osvr_OculusRift_json.h"

// Library/third-party includes
#include <json/reader.h>
#include <json/value.h>

#include <vrpn_Tracker_OculusRift.h>

// Standard includes
#include <iostream>

void createOculusRift(VRPNMultiserverData &data, OSVR_PluginRegContext ctx,
                      const char *params) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(params, root)) {
        std::cerr
            << "-- createOculusRift(): ERROR: Could not parse configuration: "
            << reader.getFormattedErrorMessages() << std::endl;
        throw std::runtime_error("Could not parse configuration: " +
                                 reader.getFormattedErrorMessages());
    }

    int hmd_index = 0;
    try {
        hmd_index = root.get("index", 0).asInt();
    } catch (const std::exception &e) {
        std::cerr << "Caught exception HMD index from config file: " << e.what()
                  << std::endl;
    }

    osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);

    reg.registerDevice(new vrpn_Tracker_OculusRift(
        reg.useDecoratedName(data.getName("OculusRift")).c_str(),
        reg.getVRPNConnection(), hmd_index));
    reg.setDeviceDescriptor(osvr::util::makeString(com_osvr_OculusRift_json));
}
