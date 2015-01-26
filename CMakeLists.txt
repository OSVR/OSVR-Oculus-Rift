#
# Oculus Rift plugin for OSVR
#
cmake_minimum_required(VERSION 2.8.12)
project(com_osvr_OculusRift)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(osvr REQUIRED)
find_package(VRPN REQUIRED)
find_package(OVR REQUIRED)
find_package(jsoncpp REQUIRED)

osvr_add_plugin(com_osvr_OculusRift com_osvr_OculusRift.cpp
    DevicesWithParameters.cpp
    DevicesWithParameters.h
    VRPNMultiserver.cpp
    VRPNMultiserver.h)

target_include_directories(com_osvr_OculusRift
    PUBLIC
    ${VRPN_INCLUDE_DIRS}
    ${OVR_INCLUDE_DIRS})

target_link_libraries(com_osvr_OculusRift
    osvr::osvrPluginKitCpp
    osvr::osvrConnection
    ${VRPN_SERVER_LIBRARIES}
    osvr::osvrPluginHost
    osvr::osvrVRPNServer
    ${OVR_LIBRARIES}
    jsoncpp_lib)

#install(FILES
#    com_osvr_OculusRift.json
#    DESTINATION "${CMAKE_INSTALL_DOCDIR}/device-descriptors"
#    COMPONENT Doc)

