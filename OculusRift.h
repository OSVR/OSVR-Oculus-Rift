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


#ifndef INCLUDED_OculusRift_h_GUID_7157AB6B_88FC_44A6_8E1A_0686DF08FCBA
#define INCLUDED_OculusRift_h_GUID_7157AB6B_88FC_44A6_8E1A_0686DF08FCBA

// Internal Includes
#include "osvr_compiler_detection.h"    // for OSVR_NOTHROW
#include "OSVR_OVR_Version.h"

// Library/third-party includes
#include <osvr/PluginKit/TrackerInterfaceC.h>
#include <osvr/PluginKit/AnalogInterfaceC.h>
#include <osvr/PluginKit/DeviceInterface.h>
#include <osvr/PluginKit/PluginKit.h>
#include <OVR_CAPI.h>

// Standard includes
#include <string>

class OculusRift {
public:
    OculusRift(OSVR_PluginRegContext ctx, int index = 0);
    ~OculusRift() OSVR_NOEXCEPT;
    void destroy();

    std::string getProductName() const;
    std::string getManufacturer() const;
    std::string getSerialNumber() const;
    std::string getFirmwareVersion() const;

    unsigned int detectTrackers();
    unsigned int getTrackerCount() const;

    std::string getDeviceDescriptorJson() const;
    std::string getDisplayJson() const;

    OSVR_ReturnCode update();

private:
    double getMonocularHorizontalFovDegrees() const;
    double getMonocularVerticalFovDegrees() const;

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,8,0,0)
    ovrSession hmd_{nullptr};
#else
    ovrHmd hmd_{nullptr};
#endif

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    ovrGraphicsLuid luid_;
#endif
    osvr::pluginkit::DeviceToken deviceToken_;
    OSVR_TrackerDeviceInterface tracker_;
    OSVR_AnalogDeviceInterface analog_;
    int numTrackers_ = 0;

    enum class TrackerChannels {
        HMD,
        CAMERA,
        LEVELED_CAMERA,
        NUM_CHANNELS
    };

    enum class AnalogChannels {
        ACCELEROMETER_X,
        ACCELEROMETER_Y,
        ACCELEROMETER_Z,
        GYROSCOPE_X,
        GYROSCOPE_Y,
        GYROSCOPE_Z,
        MAGNETOMETER_X,
        MAGNETOMETER_Y,
        MAGNETOMETER_Z,
        TEMPERATURE,
        NUM_CHANNELS
    };

};

#endif // INCLUDED_OculusRift_h_GUID_7157AB6B_88FC_44A6_8E1A_0686DF08FCBA

