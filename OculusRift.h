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
// - none

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
    ~OculusRift() noexcept;
    void destroy();

    std::string getProductName() const;
    std::string getManufacturer() const;
    std::string getSerialNumber() const;
    std::string getFirmwareVersion() const;

    std::string getDisplayJson() const;

    OSVR_ReturnCode update();

private:
    ovrHmd hmd_{nullptr};
    osvr::pluginkit::DeviceToken deviceToken_;
    OSVR_TrackerDeviceInterface hmdTracker_;
    OSVR_TrackerDeviceInterface cameraTracker_;
    OSVR_TrackerDeviceInterface leveledCameraTracker_;
    OSVR_AnalogDeviceInterface accelerometer_;

    double getMonocularHorizontalFovDegrees() const;
    double getMonocularVerticalFovDegrees() const;
};

#endif // INCLUDED_OculusRift_h_GUID_7157AB6B_88FC_44A6_8E1A_0686DF08FCBA

