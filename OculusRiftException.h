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


#ifndef INCLUDED_OculusRiftException_h_GUID_AF7C1708_4C46_42CA_B6F4_70E5644CFC63
#define INCLUDED_OculusRiftException_h_GUID_AF7C1708_4C46_42CA_B6F4_70E5644CFC63

// Internal Includes
#include "osvr_compiler_detection.h"

// Library/third-party includes
// - none

// Standard includes
#include <exception>
#include <string>

class OculusRiftException : public std::exception {
public:
	OculusRiftException(const std::string& msg) OSVR_NOEXCEPT : std::exception(), message_(msg)
	{
		// do nothing
	}

	virtual const char* what() const OSVR_OVERRIDE
	{
		return message_.c_str();
	}

private:
	const std::string message_;
};

#endif // INCLUDED_OculusRiftException_h_GUID_AF7C1708_4C46_42CA_B6F4_70E5644CFC63
