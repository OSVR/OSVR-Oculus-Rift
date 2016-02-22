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


#ifndef INCLUDED_OculusRiftDetector_h_GUID_C0699917_4E69_47D3_B128_222A734698CD
#define INCLUDED_OculusRiftDetector_h_GUID_C0699917_4E69_47D3_B128_222A734698CD

// Internal Includes
#include "OculusRiftManager.h"

// Library/third-party includes
#include <osvr/PluginKit/PluginKit.h>

// Standard includes
#include <iostream>

class OculusRiftDetector {
public:
    OculusRiftDetector();

    OSVR_ReturnCode detect(OSVR_PluginRegContext ctx);

    OSVR_ReturnCode operator()(OSVR_PluginRegContext ctx);

private:
    std::unique_ptr<OculusRiftManager> manager_;
};

inline OculusRiftDetector::OculusRiftDetector() : manager_(new OculusRiftManager)
{
    // do nothing
}

inline OSVR_ReturnCode OculusRiftDetector::detect(OSVR_PluginRegContext ctx)
{
    std::cout << "[OculusRiftDetector] Received hardware detection request." << std::endl;

    if (!manager_) {
        std::cerr << "[OculusRiftDetector] Invalid OculusRiftManager pointer." << std::endl;
        return OSVR_RETURN_FAILURE;
    }

    return manager_->detect(ctx);
}

inline OSVR_ReturnCode OculusRiftDetector::operator()(OSVR_PluginRegContext ctx)
{
    return detect(ctx);
}

#endif // INCLUDED_OculusRiftDetector_h_GUID_C0699917_4E69_47D3_B128_222A734698CD

