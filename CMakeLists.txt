#
# Oculus Rift plugin for OSVR
#
cmake_minimum_required(VERSION 2.8.12)
project(org_opengoggles_OculusRift)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(osvr REQUIRED)
find_package(VRPN REQUIRED)
find_package(OVR REQUIRED)
find_package(jsoncpp REQUIRED)

osvr_add_plugin(org_opengoggles_OculusRift org_opengoggles_OculusRift.cpp
    DevicesWithParameters.cpp
    DevicesWithParameters.h
    VRPNMultiserver.cpp
    VRPNMultiserver.h)

target_include_directories(org_opengoggles_OculusRift
    PUBLIC
    ${VRPN_INCLUDE_DIRS}
    ${OVR_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

target_link_libraries(org_opengoggles_OculusRift
    osvr::osvrPluginKitCpp
    osvr::osvrConnection
    ${VRPN_SERVER_LIBRARIES}
    osvr::osvrPluginHost
    osvr::osvrVRPNServer
    ${OVR_LIBRARIES}
    jsoncpp_lib)

#target_compile_options(org_opengoggles_OculusRift
#    PRIVATE
#    ${OSVR_CXX11_FLAGS})

#set_target_properties(org_opengoggles_OculusRift PROPERTIES
#    PREFIX ""
#    FOLDER "OSVR Plugins")

#install(TARGETS org_opengoggles_OculusRift
#    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)

#install(FILES
#    org_opengoggles_OculusRift.json
#    DESTINATION "${CMAKE_INSTALL_DOCDIR}/device-descriptors"
#    COMPONENT Doc)

