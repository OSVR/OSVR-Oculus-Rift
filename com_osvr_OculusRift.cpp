/** @file
    @brief Oculus Rift driver for OSVR.
   devices.

    @date 2015

    @author
    Kevin M. Godby
    <kevin@godby.org>
*/

// Copyright 2015 Sensics, Inc.
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
#include <osvr/PluginKit/PluginKit.h>
#include "OculusRiftDetector.h"
#include "OculusRiftManager.h"

// Library/third-party includes
// - none

// Standard includes
#include <memory>
#include <iostream>  // for debugging

OSVR_PLUGIN(com_osvr_OculusRift) {
    std::cout << "[OSVR Oculus Rift] OSVR_PLUGIN(com_osvr_OculusRift) called." << std::endl;

    osvr::pluginkit::PluginContext context{ctx};

    OculusRiftManager* oculus_rift_manager{new OculusRiftManager{}};
    osvr::pluginkit::registerObjectForDeletion(ctx, oculus_rift_manager);

    context.registerHardwareDetectCallback(new OculusRiftDetector{oculus_rift_manager});

    return OSVR_RETURN_SUCCESS;
}

