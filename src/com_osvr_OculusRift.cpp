/** @file
    @brief Oculus Rift driver for OSVR.

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
#include "OculusRiftManager.h"

// Library/third-party includes
// - none

// Standard includes
// - none

OSVR_PLUGIN(com_osvr_OculusRift)
{
    osvr::pluginkit::PluginContext context{ctx};
    context.registerHardwareDetectCallback(new OculusRiftManager{ctx});

    return OSVR_RETURN_SUCCESS;
}

