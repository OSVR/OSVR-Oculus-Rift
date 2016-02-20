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
#include "OculusRiftException.h"
#include "GetLastError.h"

// Library/third-party includes
#include <json/json.h>

// Standard includes
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

OSVR_CONSTEXPR double OSVR_PI = 3.1415926535897932385;

OculusRift::OculusRift(OSVR_PluginRegContext ctx, int index)
{
    std::cout << "[Oculus Rift] Creating Oculus Rift..." << std::endl;

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    // Connect HMD
    ovrResult result = ovr_Create(&hmd_, &luid_);
    if (OVR_FAILURE(result)) {
        const std::string msg = "Error creatinng HMD: " + getLastErrorMessage() + ".";
        std::cerr << "[Oculus Rift] " << msg << std::endl;
        throw OculusRiftException(msg);
    }
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,6,0,0)
    // Connect to HMD
    ovrResult result = ovrHmd_Create(index, &hmd_);
    if (OVR_FAILURE(result)) {
        const std::string msg = "Error creatinng HMD: " + getLastErrorMessage() + ".";
        std::cerr << "[Oculus Rift] " << msg  << std::endl;
        throw OculusRiftException(msg);
    }
#else
    // Connect to HMD
    hmd_ = ovrHmd_Create(index);
    if (!hmd_) {
        std::cerr << "[Oculus Rift] Error creating HMD handle." << std::endl;
        throw OculusRiftException("Error creating HMD handle.");
    }
#endif

    // Initialize tracking and sensor fusion
    const unsigned int supported_tracking_capabilities = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position; // the tracking capabilities this driver will report
    const unsigned int required_tracking_capabilities = 0; // the tracking capabilities which must be supported by the HMD
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const ovrBool tracking_configured = ovr_ConfigureTracking(hmd_, supported_tracking_capabilities, required_tracking_capabilities);
    // returns FALSE if the required tracking capabilities are not supported (e.g., camera isn't plugged in)
#else
    const ovrBool tracking_configured = ovrHmd_ConfigureTracking(hmd_, supported_tracking_capabilities, required_tracking_capabilities);
    // returns FALSE if the required tracking capabilities are not supported (e.g., camera isn't plugged in)
#endif

    detectTrackers();

    std::cout << getDisplayJson();

    OSVR_DeviceInitOptions opts = osvrDeviceCreateInitOptions(ctx);
    osvrDeviceTrackerConfigure(opts, &tracker_);
    osvrDeviceAnalogConfigure(opts, &analog_, static_cast<OSVR_ChannelCount>(AnalogChannels::NUM_CHANNELS));

    // Create the sync device token with the options
    std::string device_name = "OculusRift" + std::to_string(index);
    deviceToken_.initSync(ctx, device_name.c_str(), opts);
    deviceToken_.sendJsonDescriptor(getDeviceDescriptorJson());
    deviceToken_.registerUpdateCallback(this);
}

OculusRift::~OculusRift() OSVR_NOEXCEPT
{
    destroy();
}

void OculusRift::destroy()
{
    std::cout << "[Oculus Rift] Destroying Oculus Rift..." << std::endl;
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    ovr_Destroy(hmd_);
#else
    ovrHmd_Destroy(hmd_);
#endif
}

std::string OculusRift::getProductName() const
{
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const auto hmd_desc = ovr_GetHmdDesc(hmd_);
    return hmd_desc.ProductName;
#else
    return hmd_->ProductName;
#endif
}

std::string OculusRift::getManufacturer() const
{
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const auto hmd_desc = ovr_GetHmdDesc(hmd_);
    return hmd_desc.Manufacturer;
#else
    return hmd_->Manufacturer;
#endif
}

std::string OculusRift::getSerialNumber() const
{
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const auto hmd_desc = ovr_GetHmdDesc(hmd_);
    return hmd_desc.SerialNumber;
#else
    return hmd_->SerialNumber;
#endif
}

std::string OculusRift::getFirmwareVersion() const
{

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const auto hmd_desc = ovr_GetHmdDesc(hmd_);
    const auto fw_major = std::to_string(hmd_desc.FirmwareMajor);
    const auto fw_minor = std::to_string(hmd_desc.FirmwareMinor);
    return fw_major + "." + fw_minor;
#else
    return std::to_string(hmd_->FirmwareMajor) + "." + std::to_string(hmd_->FirmwareMinor);
#endif
}

unsigned int OculusRift::detectTrackers()
{
    int num_trackers = 0;

#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,8,0,0)
    const ovrTrackingState ts = ovr_GetTrackingState(hmd_, 0.0, ovrFalse);
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const ovrTrackingState ts = ovr_GetTrackingState(hmd_, 0.0);
#else
    const ovrTrackingState ts = ovrHmd_GetTrackingState(hmd_, ovr_GetTimeInSeconds());
#endif

    // Can we track the HMD?
    if (ts.StatusFlags & ovrStatus_OrientationTracked) {
        num_trackers++;
    }

    // Can we track the camera?
    if (ts.StatusFlags & ovrStatus_CameraPoseTracked) {
        num_trackers += 2; // first for camera, second for leveled camera
    }

    numTrackers_ = num_trackers;

    return numTrackers_;
}

unsigned int OculusRift::getTrackerCount() const
{
    return numTrackers_;
}

std::string OculusRift::getDeviceDescriptorJson() const
{
    Json::Value root;
    root["deviceVender"] = getManufacturer();
    root["deviceName"] = getProductName();
    root["author"] = "Kevin M. Godby <kevin@godby.org>";
    root["version"] = 1;
    root["lastModified"] = "2015-06-14T20:42:13Z";
    root["interfaces"]["tracker"]["count"] = getTrackerCount();
    root["interfaces"]["tracker"]["position"] = true; // FIXME
    root["interfaces"]["tracker"]["orientation"] = true; // FIXME
    root["interfaces"]["analog"]["count"] = 10; // FIXME
    //root["interfaces"]["analog"]["traits"] = {}; // FIXME
    root["semantic"]["hmd"]["$target"] = "tracker/0";
    root["semantic"]["hmd"]["accelerometer"]["x"] = "analog/0";
    root["semantic"]["hmd"]["accelerometer"]["y"] = "analog/1";
    root["semantic"]["hmd"]["accelerometer"]["z"] = "analog/2";
    root["semantic"]["hmd"]["gyroscope"]["x"] = "analog/3";
    root["semantic"]["hmd"]["gyroscope"]["y"] = "analog/4";
    root["semantic"]["hmd"]["gyroscope"]["z"] = "analog/5";
    root["semantic"]["hmd"]["magnetometer"]["x"] = "analog/6";
    root["semantic"]["hmd"]["magnetometer"]["y"] = "analog/7";
    root["semantic"]["hmd"]["magnetometer"]["z"] = "analog/8";
    root["semantic"]["hmd"]["temperature"] = "analog/9";
    root["semantic"]["camera"]["$target"] = "tracker/1";
    root["semantic"]["leveled_camera"]["$target"] = "tracker/2";
    root["automaticAliases"]["/me/head"] = "semantic/hmd";

    std::ostringstream ostr;
    ostr << root;

    std::cout << root;

    return ostr.str();
}

std::string OculusRift::getDisplayJson() const
{
    // Field of view
    const double monocular_horizontal = getMonocularHorizontalFovDegrees();
    const double monocular_vertical = getMonocularVerticalFovDegrees();
    const double overlap_percent = 0.0; // TODO
    const double pitch_tilt = 0.0; // TODO

    // Resolution
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const double width = ovr_GetHmdDesc(hmd_).Resolution.w;
    const double height = ovr_GetHmdDesc(hmd_).Resolution.h;
#else
    const double width = hmd_->Resolution.w;
    const double height = hmd_->Resolution.h;
#endif
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

    Json::Value root;
    root["hmd"]["field_of_view"]["monocular_horizontal"] = monocular_horizontal;
    root["hmd"]["field_of_view"]["monocular_vertical"] = monocular_vertical;
    root["hmd"]["field_of_view"]["overlap_percent"] = overlap_percent;
    root["hmd"]["field_of_view"]["pitch_tilt"] = pitch_tilt;
    root["hmd"]["resolutions"][0]["width"] = width;
    root["hmd"]["resolutions"][0]["height"] = height;
    root["hmd"]["resolutions"][0]["video_inputs"] = video_inputs;
    root["hmd"]["resolutions"][0]["display_mode"] = display_mode;
    root["hmd"]["distortion"]["k1_red"] = k1_red;
    root["hmd"]["distortion"]["k1_blue"] = k1_blue;
    root["hmd"]["distortion"]["k1_green"] = k1_green;
    root["hmd"]["rendering"]["right_roll"] = right_roll;
    root["hmd"]["rendering"]["left_roll"] = left_roll;
    root["hmd"]["eyes"][0]["center_proj_x"] = center_proj_x;
    root["hmd"]["eyes"][0]["center_proj_y"] = center_proj_y;
    root["hmd"]["eyes"][0]["rotate_180"] = rotate_180;

    std::ostringstream ostr;
    ostr << root;
    return ostr.str();
}

double OculusRift::getMonocularHorizontalFovDegrees() const
{
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const double left_eye_left_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_right_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Left].RightTan;
    const double right_eye_left_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Right].LeftTan;
    const double right_eye_right_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Right].RightTan;
#else
    const double left_eye_left_tan = hmd_->DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_right_tan = hmd_->DefaultEyeFov[ovrEye_Left].RightTan;
    const double right_eye_left_tan = hmd_->DefaultEyeFov[ovrEye_Right].LeftTan;
    const double right_eye_right_tan = hmd_->DefaultEyeFov[ovrEye_Right].RightTan;
#endif
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
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const double left_eye_up_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_down_tan = ovr_GetHmdDesc(hmd_).DefaultEyeFov[ovrEye_Left].RightTan;
#else
    const double left_eye_up_tan = hmd_->DefaultEyeFov[ovrEye_Left].LeftTan;
    const double left_eye_down_tan = hmd_->DefaultEyeFov[ovrEye_Left].RightTan;
#endif
    const double left_eye_up_degrees = std::atan(left_eye_up_tan) * 180.0 / OSVR_PI;
    const double left_eye_down_degrees = std::atan(left_eye_down_tan) * 180.0 / OSVR_PI;

    const double left_eye_fov = left_eye_up_degrees + left_eye_down_degrees;

    return left_eye_fov;
}

OSVR_ReturnCode OculusRift::update()
{
    std::cout << "[Oculus Rift] Updating tracker and analog values!" << std::endl;

    // Poll tracking data
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,8,0,0)
    const ovrTrackingState ts = ovr_GetTrackingState(hmd_, 0.0, ovrFalse);
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,7,0,0)
    const ovrTrackingState ts = ovr_GetTrackingState(hmd_, 0.0);
#else
    const ovrTrackingState ts = ovrHmd_GetTrackingState(hmd_, ovr_GetTimeInSeconds());
#endif

    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
        // Both orientation and position are known
        std::cout << "[Oculus Rift] Supports orientation and position tracking." << std::endl;

        const ovrPoseStatef head_state = ts.HeadPose;
        const ovrPosef head_pose = head_state.ThePose;

        OSVR_Pose3 hmd_pose;
        hmd_pose.translation.data[0] = head_pose.Position.x;
        hmd_pose.translation.data[1] = head_pose.Position.y;
        hmd_pose.translation.data[2] = head_pose.Position.z;
        hmd_pose.rotation.data[0] = head_pose.Orientation.w;
        hmd_pose.rotation.data[1] = head_pose.Orientation.x;
        hmd_pose.rotation.data[2] = head_pose.Orientation.y;
        hmd_pose.rotation.data[3] = head_pose.Orientation.z;

        osvrDeviceTrackerSendPose(deviceToken_, tracker_, &hmd_pose, static_cast<OSVR_ChannelCount>(TrackerChannels::HMD));
    } else if (ts.StatusFlags & (ovrStatus_OrientationTracked)) {
        // Only the orientation is known
        std::cout << "[Oculus Rift] Supports orientation tracking only." << std::endl;

        const ovrPoseStatef head_state = ts.HeadPose;
        const ovrPosef head_pose = head_state.ThePose;

        OSVR_OrientationState hmd_orientation;
        hmd_orientation.data[0] = head_pose.Orientation.w;
        hmd_orientation.data[1] = head_pose.Orientation.x;
        hmd_orientation.data[2] = head_pose.Orientation.y;
        hmd_orientation.data[3] = head_pose.Orientation.z;

        osvrDeviceTrackerSendOrientation(deviceToken_, tracker_, &hmd_orientation, static_cast<OSVR_ChannelCount>(TrackerChannels::HMD));
    } else if (ts.StatusFlags & (ovrStatus_PositionTracked)) {
        // Only the position is known
        std::cout << "[Oculus Rift] Supports position tracking only." << std::endl;

        const ovrPoseStatef head_state = ts.HeadPose;
        const ovrPosef head_pose = head_state.ThePose;

        OSVR_PositionState hmd_position;
        hmd_position.data[0] = head_pose.Position.x;
        hmd_position.data[1] = head_pose.Position.y;
        hmd_position.data[2] = head_pose.Position.z;

        osvrDeviceTrackerSendPosition(deviceToken_, tracker_, &hmd_position, static_cast<OSVR_ChannelCount>(TrackerChannels::HMD));
    }

    if (ts.StatusFlags & (ovrStatus_CameraPoseTracked)) {
        std::cout << "[Oculus Rift] Supports camera pose tracking." << std::endl;

        OSVR_Pose3 camera_pose;
        camera_pose.translation.data[0] = ts.CameraPose.Position.x;
        camera_pose.translation.data[1] = ts.CameraPose.Position.y;
        camera_pose.translation.data[2] = ts.CameraPose.Position.z;
        camera_pose.rotation.data[0] = ts.CameraPose.Orientation.w;
        camera_pose.rotation.data[1] = ts.CameraPose.Orientation.x;
        camera_pose.rotation.data[2] = ts.CameraPose.Orientation.y;
        camera_pose.rotation.data[3] = ts.CameraPose.Orientation.z;
        osvrDeviceTrackerSendPose(deviceToken_, tracker_, &camera_pose, static_cast<OSVR_ChannelCount>(TrackerChannels::CAMERA));

        OSVR_Pose3 leveled_camera_pose;
        leveled_camera_pose.translation.data[0] = ts.LeveledCameraPose.Position.x;
        leveled_camera_pose.translation.data[1] = ts.LeveledCameraPose.Position.y;
        leveled_camera_pose.translation.data[2] = ts.LeveledCameraPose.Position.z;
        leveled_camera_pose.rotation.data[0] = ts.LeveledCameraPose.Orientation.w;
        leveled_camera_pose.rotation.data[1] = ts.LeveledCameraPose.Orientation.x;
        leveled_camera_pose.rotation.data[2] = ts.LeveledCameraPose.Orientation.y;
        leveled_camera_pose.rotation.data[3] = ts.LeveledCameraPose.Orientation.z;
        osvrDeviceTrackerSendPose(deviceToken_, tracker_, &leveled_camera_pose, static_cast<OSVR_ChannelCount>(TrackerChannels::LEVELED_CAMERA));
    }

    // Now for the analog interfaces
    const ovrSensorData sensor_data = ts.RawSensorData;

    // Acceleration reading (x, y, z) in m/s^2
    const ovrVector3f accelerometer = sensor_data.Accelerometer;
    osvrDeviceAnalogSetValue(deviceToken_, analog_, accelerometer.x, static_cast<OSVR_ChannelCount>(AnalogChannels::ACCELEROMETER_X));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, accelerometer.y, static_cast<OSVR_ChannelCount>(AnalogChannels::ACCELEROMETER_Y));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, accelerometer.z, static_cast<OSVR_ChannelCount>(AnalogChannels::ACCELEROMETER_Z));

    // Rotation rate (x, y, z) in rad/s
    const ovrVector3f gyro = sensor_data.Gyro;
    osvrDeviceAnalogSetValue(deviceToken_, analog_, gyro.x, static_cast<OSVR_ChannelCount>(AnalogChannels::GYROSCOPE_X));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, gyro.y, static_cast<OSVR_ChannelCount>(AnalogChannels::GYROSCOPE_Y));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, gyro.z, static_cast<OSVR_ChannelCount>(AnalogChannels::GYROSCOPE_Z));

    // Magnetic field (x, y, z) in Gauss
    const ovrVector3f magnetometer = sensor_data.Magnetometer;
    osvrDeviceAnalogSetValue(deviceToken_, analog_, magnetometer.x, static_cast<OSVR_ChannelCount>(AnalogChannels::MAGNETOMETER_X));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, magnetometer.y, static_cast<OSVR_ChannelCount>(AnalogChannels::MAGNETOMETER_Y));
    osvrDeviceAnalogSetValue(deviceToken_, analog_, magnetometer.z, static_cast<OSVR_ChannelCount>(AnalogChannels::MAGNETOMETER_Z));

    // Temperature of sensor in degrees Celsius
    const float temperature = sensor_data.Temperature;
    osvrDeviceAnalogSetValue(deviceToken_, analog_, temperature, static_cast<OSVR_ChannelCount>(AnalogChannels::TEMPERATURE));

    return OSVR_RETURN_SUCCESS;
}

