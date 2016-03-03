/** @file
    @brief Get the last error from the Oculus SDK.

    @date 2016

    @author
    Sensics, Inc.
    <http://sensics.com>

*/

// Copyright 2016 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDED_GetLastError_h_GUID_2DAD1C8D_FC01_4ABD_9130_D37C45B46867
#define INCLUDED_GetLastError_h_GUID_2DAD1C8D_FC01_4ABD_9130_D37C45B46867

// Internal Includes
#include "OSVR_OVR_Version.h"

// Library/third-party includes
#include <OVR_CAPI.h>

// Standard includes
#include <string>

/**
 * @brief Returns a string containing the most recent error message from the
 * Oculus SDK.
 *
 * @return a string containing the most recent error message from the Oculus
 * SDK.
 */
inline std::string getLastErrorMessage()
{
    std::string error_msg;
#if OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,6,0,0)
    ovrErrorInfo error_info;
    ovr_GetLastErrorInfo(&error_info);
    error_msg = std::string(error_info.ErrorString);
#elif OSVR_OVR_VERSION_GREATER_OR_EQUAL(0,5,0,0)
    const char* msg = ovrHmd_GetLastError(nullptr);
    error_msg = std::string(msg);
#endif
    return error_msg;
}

#endif // INCLUDED_GetLastError_h_GUID_2DAD1C8D_FC01_4ABD_9130_D37C45B46867

