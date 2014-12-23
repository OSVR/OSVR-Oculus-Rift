/** @file
    @brief Implementation of the "multiserver" plugin that offers the stock VRPN
   devices.

    @date 2014

    @author
    Kevin M. Godby
    <kevin@godby.org>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include "VRPNMultiserver.h"
#include "DevicesWithParameters.h"
#include <osvr/PluginKit/PluginKit.h>
#include <osvr/VRPNServer/VRPNDeviceRegistration.h>

// Library/third-party includes
#include "hidapi/hidapi.h"
#include "vrpn_Connection.h"
#include "vrpn_Tracker_OculusRift.h"
#include <boost/noncopyable.hpp>
#include <json/reader.h>
#include <json/value.h>

// Standard includes
#include <iostream>

class VRPNHardwareDetect : boost::noncopyable {
public:
    VRPNHardwareDetect(VRPNMultiserverData& data) : m_data(data) {}
    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx) {
        struct hid_device_info *enumData = hid_enumerate(0, 0);
        for (struct hid_device_info *dev = enumData; dev != nullptr; dev = dev->next) {
            if (dev->vendor_id != 0x2833)
                continue;

            // Oculus Rift
            osvr::vrpnserver::VRPNDeviceRegistration reg(ctx);
            std::string name = reg.useDecoratedName(m_data.getName("OculusRift"));
            int hmd_index = 0;
            const char hmd_type[] = "dk2";
            reg.registerDevice(new vrpn_Tracker_OculusRift(name.c_str(), reg.getVRPNConnection(), hmd_index, hmd_type));
        }
        hid_free_enumeration(enumData);
        return OSVR_RETURN_SUCCESS;
    }

private:
    VRPNMultiserverData &m_data;
};

OSVR_PLUGIN(org_opengoggles_bundled_Multiserver) {
    std::cout << "-- Creating plugin context..." << std::endl;
    osvr::pluginkit::PluginContext context(ctx);

    std::cout << "-- Registering VRPN multiserver data..." << std::endl;
    VRPNMultiserverData &data = *context.registerObjectForDeletion(new VRPNMultiserverData);

    std::cout << "-- Registering hardware detection..." << std::endl;
    context.registerHardwareDetectCallback(new VRPNHardwareDetect(data));

    //std::cout << "-- Registering driver instantiation callback..." << std::endl;
    //osvrRegisterDriverInstantiationCallback(ctx, "Oculus Rift", &wrappedConstructor<&createOculusRift>, &data);

    return OSVR_RETURN_SUCCESS;
}

