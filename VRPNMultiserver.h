/** @file
    @brief Header

    @date 2014

    @author
    Ryan Pavlik
    <ryan@sensics.com>
    <http://sensics.com>
*/

// Copyright 2014 Sensics, Inc.
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

#ifndef INCLUDED_VRPNMultiserver_h_GUID_05913DEF_7B12_4089_FE38_379BEF4A1A7D
#define INCLUDED_VRPNMultiserver_h_GUID_05913DEF_7B12_4089_FE38_379BEF4A1A7D

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <map>
#include <string>

class VRPNMultiserverData {
  public:
    std::string getName(std::string const &nameStem);

  private:
    typedef std::map<std::string, size_t> NameCountMap;
    std::string assignName(std::string const &nameStem);
    size_t assignNumber(std::string const &nameStem);

    NameCountMap m_nameCount;
};

#endif // INCLUDED_VRPNMultiserver_h_GUID_05913DEF_7B12_4089_FE38_379BEF4A1A7D
