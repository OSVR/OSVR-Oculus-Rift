#
# Oculus Rift plugin for OSVR
#
cmake_minimum_required(VERSION 3.1.0)
project(com_osvr_OculusRift)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Dependencies
#
find_package(osvr REQUIRED)
find_package(OVR REQUIRED)
find_package(jsoncpp REQUIRED)

#
# Detect available C++ language, library, and compiler features
#
include(WriteCompilerDetectionHeader)
write_compiler_detection_header(
	FILE "osvr_compiler_detection.h"
	PREFIX OSVR
	COMPILERS GNU Clang AppleClang MSVC
	FEATURES cxx_override cxx_noexcept cxx_constexpr
)

include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_CXX11_STANDARD_COMPILE_OPTION})
check_cxx_source_compiles("#include <memory>\nint main(int, char**) { std::unique_ptr<int> i = std::make_unique<int>(0); }" OSVR_HAS_STD_MAKE_UNIQUE)
configure_file(osvr_compiler_tests.h.in "${CMAKE_CURRENT_BINARY_DIR}/osvr_compiler_tests.h")


osvr_add_plugin(com_osvr_OculusRift
	com_osvr_OculusRift.cpp
	OculusRift.cpp
	contains.h
	OculusRiftDetector.h
	OculusRiftException.h
	OculusRift.h
	OculusRiftManager.h
	ovr_version.h
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

