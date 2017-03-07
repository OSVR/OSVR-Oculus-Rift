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

// Internal Includes
#include "OculusRiftManager.h"
#include "OculusRift.h"
#include "OSVR_OVR_Version.h"
#include "make_unique.h"
#include "GetLastError.h"

// Library/third-party includes
#include <OVR_CAPI.h>
#include <osvr/PluginKit/PluginKit.h>

// Standard includes
#include <iostream>
#include <string>

OculusRiftManager::OculusRiftManager(OSVR_PluginRegContext context) : context_(context)
{
    initialize();
}

OculusRiftManager::~OculusRiftManager()
{
    shutdown();
}

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
void ovr_log_callback(uintptr_t plugin_context, int level, const char* message)
{
    auto ctx = reinterpret_cast<osvr::pluginkit::PluginContext*>(plugin_context);
    OSVR_LogLevel severity = OSVR_LOGLEVEL_INFO;
    if (ovrLogLevel_Debug == level) {
        severity = OSVR_LOGLEVEL_DEBUG;
    } else if (ovrLogLevel_Info == level) {
        severity = OSVR_LOGLEVEL_INFO;
    } else if (ovrLogLevel_Error == level) {
        severity = OSVR_LOGLEVEL_ERROR;
    }
    ctx->log(severity, message)
    std::cerr << "[Oculus Rift] " << message << std::endl;
}
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
void ovr_log_callback(int level, const char* message)
{
    std::cerr << "[OVR " << level << "] " << message << std::endl;
}
#endif

bool OculusRiftManager::initialize()
{
    context_.log(OSVR_LOGLEVEL_TRACE, "Initializing Oculus API...");

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    ovrInitParams params = {
        ovrInit_Debug | ovrInit_RequestVersion, // Flags
        OVR_MINOR_VERSION,      // RequestedMinorVersion
        ovr_log_callback,       // LogCallback
        &context_,              // UserData for LogCallback
        0                       // ConnectionTimeoutSeconds
    };
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
    ovrInitParams params = {
        ovrInit_Debug | ovrInit_RequestVersion, // Flags
        OVR_MINOR_VERSION,      // RequestedMinorVersion
        ovr_log_callback,       // LogCallback
        0                       // ConnectionTimeoutSeconds
    };
#endif

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,6,0,0)
    ovrResult result = ovr_Initialize(&params);
    initialized_ = OVR_SUCCESS(result);
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
        context_.log(OSVR_LOGLEVEL_TRACE, "Oculus Rift initialized.");
    } else {
        const auto msg = "Error initializing Oculus Rift system: " + getLastErrorMessage() + ".";
        context_.log(OSVR_LOGLEVEL_ERROR, msg.c_str());
    }

    return initialized_;
}

void OculusRiftManager::shutdown()
{
    context_.log(OSVR_LOGLEVEL_TRACE, "Shutting down Oculus API...");
    ovr_Shutdown();
}

OSVR_ReturnCode OculusRiftManager::detect(OSVR_PluginRegContext ctx)
{
    context_.log(OSVR_LOGLEVEL_TRACE, "Detecting Oculus Rifts...");
    if (!initialized_) {
        // Initialize the Oculus API and get a count of connected HMDs.
        context_.log(OSVR_LOGLEVEL_WARN, "OVR system not initialized. Initializing...");
        initialize();
    }

    // Detect attached HMDs.
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    ovrHmdDesc hmd_description = ovr_GetHmdDesc(nullptr);
    const int num_hmds_detected = (hmd_description.Type == ovrHmd_None ? 0 : 1);
#else
    const int num_hmds_detected = ovrHmd_Detect();
#endif
    const auto msg = "Detected " + std::to_string(num_hmds_detected) + (num_hmds_detected != 1 ? " HMDs." : " HMD.");
    context_.log(OSVR_LOGLEVEL_TRACE, msg.c_str());

    // If not HMDs were detected and we still have a valid handle to one, we
    // should release that handle as the HMD has been unplugged.
    if (num_hmds_detected == 0 && oculusRift_)
        oculusRift_.reset();

    if (num_hmds_detected > 0) {
        if (!oculusRift_) {
            // If we detect an HMD and don't already have one created, we should
            // create it.
            const int index = 0;
            try {
                oculusRift_ = std::make_unique<OculusRift>(ctx, index);
            } catch (...) {
                // do nothing
            }
        } else {
            // If we detect an HMD and we already have on created, check to see
            // if they're the same. If they are, do nothing. If they're
            // different, delete the old one and add the new one.
            // TODO
        }
    }

    return OSVR_RETURN_SUCCESS;
}

OSVR_ReturnCode OculusRiftManager::operator()(OSVR_PluginRegContext ctx)
{
    return detect(ctx);
}

