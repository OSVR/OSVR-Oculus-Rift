/** @file
    @brief Header

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

#ifndef INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143
#define INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143

// Internal Includes
#include "VRPNMultiserver.h"

// Library/third-party includes
#include <osvr/Util/PluginCallbackTypesC.h>

// Standard includes
// - none

typedef void (*DriverConstructor)(VRPNMultiserverData &data, OSVR_PluginRegContext ctx, const char *params);

template <DriverConstructor F>
OSVR_ReturnCode wrappedConstructor(OSVR_PluginRegContext ctx,
                                   const char *params, void *userData) {
    VRPNMultiserverData *data = static_cast<VRPNMultiserverData *>(userData);
    try {
        F(*data, ctx, params);
        return OSVR_RETURN_SUCCESS;
    } catch (...) {
        return OSVR_RETURN_FAILURE;
    }
}

void createOculusRift(VRPNMultiserverData &data, OSVR_PluginRegContext ctx, const char *params);

#endif // INCLUDED_DevicesWithParameters_h_GUID_45B7C6E9_990B_4FF0_9EB2_209089781143
