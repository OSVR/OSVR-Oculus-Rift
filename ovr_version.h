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


#ifndef INCLUDED_ovr_version_h_GUID_7DA9A7F4_6465_461A_A50D_D37A65253F6B
#define INCLUDED_ovr_version_h_GUID_7DA9A7F4_6465_461A_A50D_D37A65253F6B

// Internal Includes
// - none

// Library/third-party includes
#include "OVR_Version.h"

// Standard includes
// - none

#ifdef OVR_PRODUCT_VERSION
    // We're running at least 0.5.x.y
    #define OSVR_OVR_PRODUCT_VERSION OVR_PRODUCT_VERSION
    #define OSVR_OVR_MAJOR_VERSION   OVR_MAJOR_VERSION
    #define OSVR_OVR_MINOR_VERSION   OVR_MINOR_VERSION
    #define OSVR_OVR_PATCH_VERSION   OVR_PATCH_VERSION
    #define OSVR_OVR_BUILD_NUMBER    OVR_BUILD_NUMBER
#else
    // We're running version 0.4.x or earlier
    #define OSVR_OVR_PRODUCT_VERSION OVR_MAJOR_VERSION
    #define OSVR_OVR_MAJOR_VERSION   OVR_MINOR_VERSION
    #define OSVR_OVR_MINOR_VERSION   OVR_BUILD_VERSION
    #define OSVR_OVR_PATCH_VERSION   0
    #define OSVR_OVR_BUILD_NUMBER    0
#endif

/**
 * Helper function that takes four components of a version number (each
 * component must be a positive integer in the range [0, 99]) and generates a
 * single integer value in its place. For example, OSVR_MAKE_VERSION(1, 2, 3, 4)
 * would return 1020304.
 */
#define OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH) \
    (PRODUCT * 1000000 + \
     MAJOR   * 10000 + \
     MINOR   * 100 + \
     PATCH)

/**
 * Defines the Oculus SDK version as a single integer.
 */
#define OSVR_OVR_VERSION \
    OSVR_MAKE_VERSION(OSVR_OVR_PRODUCT_VERSION, \
                      OSVR_OVR_MAJOR_VERSION, \
                      OSVR_OVR_MINOR_VERSION, \
                      OSVR_OVR_PATCH_VERSION)

/**
 *  Convenience macros that can be used to specify a required version of the
 * Oculus Rift SDK.
 *
 * #if OSVR_OVR_MIN_VERSION_REQUIRED(0, 5, 0, 1)
 *    your code here
 * #endif
 */
//@{
#define OSVR_OVR_MIN_VERSION_REQUIRED(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH) >= OSVR_OVR_VERSION)

#define OSVR_OVR_VERSION_LESS_THAN(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_OVR_VERSION < OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH))

#define OSVR_OVR_VERSION_LESS_OR_EQUAL(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_OVR_VERSION <= OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH))

#define OSVR_OVR_VERSION_GREATER_THAN(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_OVR_VERSION > OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH))

#define OSVR_OVR_VERSION_GREATER_OR_EQUAL(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_OVR_VERSION >= OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH))

#define OSVR_OVR_VERSION_EQUAL(PRODUCT, MAJOR, MINOR, PATCH) \
    (OSVR_OVR_VERSION == OSVR_MAKE_VERSION(PRODUCT, MAJOR, MINOR, PATCH))
//@}

#endif // INCLUDED_ovr_version_h_GUID_7DA9A7F4_6465_461A_A50D_D37A65253F6B

