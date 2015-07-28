/** @file
    @brief Header

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
#include "contains.h"
#include "ovr_version.h"
#include "make_unique.h"

// Library/third-party includes
#include <OVR_CAPI.h>
#include <osvr/PluginKit/PluginKit.h>

// Standard includes
#include <iostream>
#include <vector>
#include <memory>

class OculusRiftManager;
using OculusRiftManagerSharedPtr = std::shared_ptr<OculusRiftManager>;
using OculusRiftManagerPtr = std::unique_ptr<OculusRiftManager>;

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

    bool isRegistered(const std::string& serial_number) const;
    void deregisterDisconnected(const std::vector<std::string>& serial_numbers);

private:
    bool initialized_{false};
    std::vector<std::unique_ptr<OculusRift>> oculusRifts_{};
};

inline OculusRiftManager::OculusRiftManager()
{
    initialize();
}

inline OculusRiftManager::~OculusRiftManager()
{
    shutdown();
}

void ovr_log_callback(int level, const char* message)
{
    std::cerr << "[OVR " << level << "] " << message << std::endl;
}

inline bool OculusRiftManager::initialize()
{
    std::cout << "[OSVR Oculus Rift] Initializing Oculus API..." << std::endl;

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
    ovrInitParams params = {
        ovrInit_Debug | ovrInit_RequestVersion, // Flags
        OVR_MINOR_VERSION,      // RequestedMinorVersion
        ovr_log_callback,       // LogCallback
        0                       // ConnectionTimeoutSeconds
    };
#endif

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,6,0,0)
    ovrResult result = ovr_Initialize(&params);
    initialized_ = OVR_SUCCESS(ovrResult);
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
    ovrBool result = ovr_Initialize(&params);
    initialized_ = static_cast<bool>(result);
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,4,4,0)
    ovrBool result = ovr_Initialize();
    initialized_ = static_cast<bool>(result);
#else
#error "Unsupported version of Oculus VR SDK. Please upgrade."
#endif

    if (initialized_) {
        std::cout << "[OSVR Oculus Rift] Oculus Rift initialized." << std::endl;
    } else {
        std::string ovr_error_msg = "[OSVR Oculus Rift] Error initializing Oculus Rift system";
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,6,0,0)
        ovrErrorInfo error_info;
        ovr_GetLastErrorInfo(&error_info);
        evr_error_msg += std::string(": ") + error_info.ErrorString;
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
        const char* msg = ovrHmd_GetLastError(nullptr);
        ovr_error_msg += std::string(": ") + msg;
#endif
        std::cerr << ovr_error_msg << "." << std::endl;
    }

    return initialized_;
}

inline void OculusRiftManager::shutdown()
{
    std::cout << "[OSVR Oculus Rift] Shutting down Oculus API..." << std::endl;
    ovr_Shutdown();
}

inline OSVR_ReturnCode OculusRiftManager::detect(OSVR_PluginRegContext ctx)
{
    std::cout << "[OSVR Oculus Rift] Detecting Oculus Rifts..." << std::endl;
    if (!initialized_) {
        // Initialize the Oculus API and get a count of connected HMDs.
        std::cout << "[OSVR Oculus Rift] OVR system not initialized. Initializing..." << std::endl;
        initialize();
    }

    // Detect attached HMDs.
    const int num_hmds_detected = ovrHmd_Detect();
    if (oculusRifts_.empty()) {
        std::cout << "[OSVR Oculus Rift] Empty HMD vector." << std::endl;
    }
    std::cout << "[OSVR Oculus Rift] Previously detected " << oculusRifts_.size() << " Oculus Rifts." << std::endl;
    std::cout << "[OSVR Oculus Rift] Detected " << num_hmds_detected << " HMDs." << std::endl;

    // Keep track of serial numbers of present HMDs so we can remove any that
    // have been detached.
    std::vector<std::string> serial_numbers;

    // Iterate over all the detected HMDs, constructing newly attached HMDs and
    // removing detached HMDs.
    for (int index = 0; index < num_hmds_detected; ++index) {
        //std::unique_ptr<OculusRift> hmd {new OculusRift{ctx, index}};
        auto hmd = std::make_unique<OculusRift>(ctx, index);

        // Retrieve the serial number of the HMD.  We'll compare it against our
        // list to add and remove newly attached or detached HMDs.
        const std::string serial_number = hmd->getSerialNumber();
        serial_numbers.emplace_back(serial_number);

        // Check to see if the HMD has already been registered
        if (isRegistered(serial_number)) {
            std::cout << "[OSVR Oculus Rift] HMD with serial number " << serial_number << " has already been registered." << std::endl;
            continue;
        }

        // Register the HMD
        std::cout << "[OSVR Oculus Rift] Registering HMD with serial number " << serial_number << "..." << std::endl;
        oculusRifts_.emplace_back(std::move(hmd));
    }

    // Now check to see if any previously discovered HMDs have been detached.
    deregisterDisconnected(serial_numbers);

    return OSVR_RETURN_SUCCESS;
}

bool OculusRiftManager::isRegistered(const std::string& serial_number) const
{
    for (const auto& hmd : oculusRifts_) {
        if (serial_number == hmd->getSerialNumber())
            return true;
    }

    return false;
}

void OculusRiftManager::deregisterDisconnected(const std::vector<std::string>& serial_numbers)
{
    std::cout << "[OSVR Oculus Rift] Searching for detached HMDs..." << std::endl;
    auto is_dead = [&serial_numbers](const std::unique_ptr<OculusRift>& hmd) {
        return !contains(serial_numbers, hmd->getSerialNumber());
    };

    const auto count_before = oculusRifts_.size();
    oculusRifts_.erase(std::remove_if(oculusRifts_.begin(), oculusRifts_.end(), is_dead), oculusRifts_.end());
    const auto count_after = oculusRifts_.size();
    std::cout << "[OSVR Oculus Rift] Deregistered " << (count_after - count_before) << " Oculus Rifts." << std::endl;
}


#endif // INCLUDED_OculusRiftManager_h_GUID_C573D70C_AA30_426B_BB75_8C30A96711A4

