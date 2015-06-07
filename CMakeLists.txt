#
# Oculus Rift plugin for OSVR
#
cmake_minimum_required(VERSION 2.8.12)
project(com_osvr_OculusRift)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(osvr REQUIRED)
find_package(OVR REQUIRED)
find_package(jsoncpp REQUIRED)

osvr_add_plugin(com_osvr_OculusRift
	OculusRift.cpp
	com_osvr_OculusRift.cpp
)

target_include_directories(com_osvr_OculusRift
	PRIVATE
	${OVR_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(com_osvr_OculusRift
	osvr::osvrPluginKitCpp
	${OVR_LIBRARIES}
	jsoncpp_lib)

install(FILES
	CONTRIBUTING.md
	README.md
	LICENSE
	NOTICE
	DESTINATION .)

