/** @file
    @brief The Oculus Rift Manager detects and sets up Oculus Rifts.

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com>

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


#ifndef INCLUDED_OculusRiftManager_h_GUID_C573D70C_AA30_426B_BB75_8C30A96711A4
#define INCLUDED_OculusRiftManager_h_GUID_C573D70C_AA30_426B_BB75_8C30A96711A4

// Internal Includes
#include "OculusRift.h"

// Library/third-party includes
#include <osvr/PluginKit/PluginKit.h>

// Standard includes
#include <memory>

class OculusRiftManager {
public:
    OculusRiftManager();
    ~OculusRiftManager();

    // Oculus Rift Manager is non-copyable
    OculusRiftManager(const OculusRiftManager&) = delete;
    OculusRiftManager& operator=(const OculusRiftManager&) = delete;

    bool initialize();
    void shutdown();

    OSVR_ReturnCode detect(OSVR_PluginRegContext ctx);
    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx);

private:
    bool initialized_{false};
    std::unique_ptr<OculusRift> oculusRift_;
};

#endif // INCLUDED_OculusRiftManager_h_GUID_C573D70C_AA30_426B_BB75_8C30A96711A4

