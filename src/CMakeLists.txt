#
# Oculus Rift plugin for OSVR
#

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

#
# Build the plugin
#
osvr_add_plugin(com_osvr_OculusRift
	com_osvr_OculusRift.cpp
	GetLastError.h
	make_unique.h
	OculusRift.cpp
	OculusRiftException.h
	OculusRift.h
	OculusRiftManager.cpp
	OculusRiftManager.h
	osvr_compiler_tests.h.in
	OSVR_OVR_Version.h
)

target_include_directories(com_osvr_OculusRift
	PRIVATE
	${OVR_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(com_osvr_OculusRift
	osvr::osvrPluginKitCpp
	${OVR_LIBRARIES}
	JsonCpp::JsonCpp)

