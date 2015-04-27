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


osvr_convert_json(com_osvr_OculusRift_json
    com_osvr_OculusRift.json
    "${CMAKE_CURRENT_BINARY_DIR}/com_osvr_OculusRift_json.h")

osvr_add_plugin(com_osvr_OculusRift com_osvr_OculusRift.cpp
    DevicesWithParameters.cpp
    DevicesWithParameters.h
    VRPNMultiserver.cpp
    VRPNMultiserver.h
    "${CMAKE_CURRENT_BINARY_DIR}/com_osvr_OculusRift_json.h")

target_include_directories(com_osvr_OculusRift
    PRIVATE
    ${VRPN_INCLUDE_DIRS}
    ${OVR_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(com_osvr_OculusRift
    osvr::osvrPluginKitCpp
    osvr::osvrVRPNServer
    ${VRPN_SERVER_LIBRARIES}
    ${OVR_LIBRARIES}
    jsoncpp_lib)

#install(FILES
#    com_osvr_OculusRift.json
#    DESTINATION "${CMAKE_INSTALL_DOCDIR}/device-descriptors"
#    COMPONENT Doc)

install(FILES
    osvr_server_config.oculusrift.sample.json
    CONTRIBUTING.md
    README.md
    LICENSE
    NOTICE
    DESTINATION .)

