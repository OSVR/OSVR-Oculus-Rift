#
# Oculus Rift plugin for OSVR
#
cmake_minimum_required(VERSION 2.8.12)
project(org_opengoggles_OculusRift)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#include(CompilerFeatures)

find_package(osvr REQUIRED)
find_package(VRPN REQUIRED)
find_package(OVR REQUIRED)
find_package(HIDAPI REQUIRED)

# This is just a helper function wrapping CMake's add_library command that
# sets up include dirs, libraries, and naming convention (no leading "lib")
# for an OSVR plugin. Pass as many source files as you need.
osvr_add_plugin(org_opengoggles_OculusRift org_opengoggles_OculusRift.cpp
    DevicesWithParameters.cpp
    DevicesWithParameters.h
    VRPNMultiserver.cpp
    VRPNMultiserver.h)

# This example uses the C++ wrapper, as suggested, so you'll need this line.
target_link_libraries(org_opengoggles_OculusRift osvr::osvrPluginKitCpp)

# If you use other libraries, find them and add a line like:
# target_link_libraries(org_opengoggles_OculusRift AnyOtherLibraries)
#target_link_libraries(org_opengoggles_OculusRift osvr::osvrConnection jsoncpp_lib vendored-vrpn)
target_link_libraries(org_opengoggles_OculusRift osvr::osvrConnection)
target_link_libraries(org_opengoggles_OculusRift ${VRPN_SERVER_LIBRARIES} ${VRPN_LIBRARIES})
target_link_libraries(org_opengoggles_OculusRift ${OVR_LIBRARIES})
target_link_libraries(org_opengoggles_OculusRift ${HIDAPI_LIBRARIES})

# If you access the C++ API that's not really designed for plugin consumption, you'll need this line:
target_link_libraries(org_opengoggles_OculusRift osvr::osvrPluginHost)
target_link_libraries(org_opengoggles_OculusRift osvr::osvrVRPNServer)

#target_compile_options(org_opengoggles_OculusRift
#    PRIVATE
#    ${OSVR_CXX11_FLAGS})


#target_link_libraries(org_opengoggles_bundled_OculusRift osvrPluginKitCpp osvrConnection osvrPluginHost jsoncpp_lib vendored-vrpn)
#target_include_directories(org_opengoggles_bundled_OculusRift PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/vrpn/submodules/hidapi")
#set_target_properties(org_opengoggles_bundled_OculusRift PROPERTIES
#    PREFIX ""
#    FOLDER "OSVR Plugins")
#
#install(TARGETS org_opengoggles_bundled_OculusRift
#    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
#
#install(FILES
#    org_opengoggles_bundled_OculusRift.json
#    DESTINATION "${CMAKE_INSTALL_DOCDIR}/device-descriptors"
#    COMPONENT Doc)

