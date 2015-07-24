/** @file
    @brief Implementation

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
#include "OculusRift.h"
#include "osvr_compiler_detection.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

OSVR_CONSTEXPR double OSVR_PI = 3.1415926535897932385;

OculusRift::OculusRift(OSVR_PluginRegContext ctx, int index)
{
    std::cout << "[Oculus Rift] Creating Oculus Rift..." << std::endl;

    OSVR_DeviceInitOptions opts = osvrDeviceCreateInitOptions(ctx);
    osvrDeviceTrackerConfigure(opts, &hmdTracker_);
    osvrDeviceTrackerConfigure(opts, &cameraTracker_);
    osvrDeviceTrackerConfigure(opts, &leveledCameraTracker_);

    // Create the sync device token with the options
    deviceToken_.initSync(ctx, "Oculus Rift", opts);
    deviceToken_.sendJsonDescriptor(getDisplayJson());
    deviceToken_.registerUpdateCallback(this);

    // Connect to HMD
    //hmd_ = ovrHmd_Create(index); // old version
	ovrResult result = ovrHmd_Create(index, &hmd_);
}

OculusRift::~OculusRift() OSVR_NOEXCEPT
{
    std::cout << "[Oculus Rift] Destroying Oculus Rift..." << std::endl;
    ovrHmd_Destroy(hmd_);
}

void OculusRift::destroy()
{
    ovrHmd_Destroy(hmd_);
}

std::string OculusRift::getProductName() const
{
    return hmd_->ProductName;
}

std::string OculusRift::getManufacturer() const
{
    return hmd_->Manufacturer;
}

std::string OculusRift::getSerialNumber() const
{
    return hmd_->SerialNumber;
}

std::string OculusRift::getFirmwareVersion() const
{
    return std::to_string(hmd_->FirmwareMajor) + "." + std::to_string(hmd_->FirmwareMinor);
}

std::string OculusRift::getDisplayJson() const
{
    // Field of view
    const double monocular_horizontal = getMonocularHorizontalFovDegrees();
    const double monocular_vertical = getMonocularVerticalFovDegrees();
    const double overlap_percent = 0.0; // TODO
    const double pitch_tilt = 0.0; // TODO

    // Resolution
    const double width = hmd_->Resolution.w;
    const double height = hmd_->Resolution.h;
    const double video_inputs = 1;
    const std::string display_mode = "horz_side_by_side";

    // Distortion
    const double k1_red = 0.0; // TODO
    const double k1_green = 0.0; // TODO
    const double k1_blue = 0.0; // TODO

    // Rendering
    const double right_roll = 0.0;
    const double left_roll = 0.0;

    // Eyes
    const double center_proj_x = 0.5; // TODO
    const double center_proj_y = 0.5; // TODO
    const bool rotate_180 = false; // TODO

    std::ostringstream ostr;
    ostr << "{\n";
    ostr << "\t\"hmd\": {\n";
    ostr << "\t\t\"field_of_view\": {\n";
    ostr << "\t\t\t\"monocular_horizontal\": " << monocular_horizontal << ",\n";
    ostr << "\t\t\t\"monocular_vertical\": " << monocular_vertical << ",\n";
    ostr << "\t\t\t\"overlap_percent\": " << overlap_percent << ",\n";
    ostr << "\t\t\t\"pitch_tilt\": " << pitch_tilt << ",\n";
    ostr << "\t\t}\n";
    ostr << "\t\t\"resolutions\": [\n";
    ostr << "\t\t\t{\n";
    ostr << "\t\t\t\t\"width\": " << width << ",\n";
    ostr << "\t\t\t\t\"height\": " << height << ",\n";
    ostr << "\t\t\t\t\"video_inputs\": " << video_inputs << ",\n";
    ostr << "\t\t\t\t\"display_mode\": \"" << display_mode << "\",\n";
    ostr << "\t\t\t}\n";
    ostr << "\t\t],\n";
    ostr << "\t\t\"distortion\": {\n";
    ostr << "\t\t\t\"k1_red\": " << k1_red << ",\n";
    ostr << "\t\t\t\"k1_green\": " << k1_green << ",\n";
    ostr << "\t\t\t\"k1_blue\": " << k1_blue << ",\n";
    ostr << "\t\t},\n";
    ostr << "\t\t\"rendering\": {\n";
    ostr << "\t\t\t\"right_roll\": " << right_roll << ",\n";
    ostr << "\t\t\t\"left_roll\": " << left_roll << "\n";
    ostr << "\t\t},\n";
    ostr << "\t\t\"eyes\": [\n";
    ostr << "\t\t\t{\n";
    ostr << "\t\t\t\t\"center_proj_x\": " << center_proj_x << ",\n";
    ostr << "\t\t\t\t\"center_proj_y\": " << center_proj_x << ",\n";
    ostr << "\t\t\t\t\"rotate_180\": " << rotate_180 << "\n";
    ostr << "\t\t\t}\n";
    ostr << "\t\t],\n";
    ostr << "\t}\n";
    ostr << "}\n";

    return ostr.str();
}

double OculusRift::getMonocularHorizontalFovDegrees() const
{
    const double left_eye_left_tan = hmd_->DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_right_tan = hmd_->DefaultEyeFov[ovrEye_Left].RightTan;
    const double right_eye_left_tan = hmd_->DefaultEyeFov[ovrEye_Right].LeftTan;
    const double right_eye_right_tan = hmd_->DefaultEyeFov[ovrEye_Right].RightTan;

    const double left_eye_left_degrees = std::atan(left_eye_left_tan) * 180.0 / OSVR_PI;
    const double left_eye_right_degrees = std::atan(left_eye_right_tan) * 180.0 / OSVR_PI;
    const double right_eye_left_degrees = std::atan(right_eye_left_tan) * 180.0 / OSVR_PI;
    const double right_eye_right_degrees = std::atan(right_eye_right_tan) * 180.0 / OSVR_PI;

    const double left_eye_fov = left_eye_left_degrees + left_eye_right_degrees;
    const double right_eye_fov = right_eye_left_degrees + right_eye_right_degrees;

    return left_eye_fov + right_eye_fov;
}

double OculusRift::getMonocularVerticalFovDegrees() const
{
    const double left_eye_up_tan = hmd_->DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_down_tan = hmd_->DefaultEyeFov[ovrEye_Left].RightTan;

    const double left_eye_up_degrees = std::atan(left_eye_up_tan) * 180.0 / OSVR_PI;
    const double left_eye_down_degrees = std::atan(left_eye_down_tan) * 180.0 / OSVR_PI;

    const double left_eye_fov = left_eye_up_degrees + left_eye_down_degrees;

    return left_eye_fov;
}

OSVR_ReturnCode OculusRift::update()
{
    return OSVR_RETURN_SUCCESS;
}

