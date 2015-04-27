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
#include "VRPNMultiserver.h"
#include "DevicesWithParameters.h"
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>

// Library/third-party includes
#include "vrpn_Connection.h"
#include "vrpn_Tracker_OculusRift.h"

// Standard includes
// - none

OSVR_PLUGIN(com_osvr_bundled_Multiserver) {
    osvr::pluginkit::PluginContext context(ctx);

    VRPNMultiserverData &data =
        *context.registerObjectForDeletion(new VRPNMultiserverData);
    osvrRegisterDriverInstantiationCallback(
        ctx, "OculusRift", &wrappedConstructor<&createOculusRift>, &data);

    return OSVR_RETURN_SUCCESS;
}
