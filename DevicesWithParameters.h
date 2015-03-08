/** @file
    @brief Header

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

#ifndef INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143
#define INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143

// Internal Includes
#include "VRPNMultiserver.h"

// Library/third-party includes
#include <osvr/Util/PluginCallbackTypesC.h>

// Standard includes
// - none

typedef void (*DriverConstructor)(VRPNMultiserverData &data, OSVR_PluginRegContext ctx, const char *params);

template <DriverConstructor F>
OSVR_ReturnCode wrappedConstructor(OSVR_PluginRegContext ctx, const char *params, void *userData) {
    VRPNMultiserverData *data = static_cast<VRPNMultiserverData *>(userData);
    try {
        F(*data, ctx, params);
        return OSVR_RETURN_SUCCESS;
    } catch (...) {
        return OSVR_RETURN_FAILURE;
    }
}

void createOculusRift(VRPNMultiserverData &data, OSVR_PluginRegContext ctx, const char *params);

#endif // INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143

