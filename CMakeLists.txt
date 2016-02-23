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
# Plugin
#
add_subdirectory(src)

#
# Documentation
#
include(GNUInstallDirs)
if(NOT CMAKE_INSTALL_DOCDIR)
	set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME})
endif()

install(FILES
	CONTRIBUTING.md
	README.md
	LICENSE
	NOTICE
	DESTINATION
	${CMAKE_INSTALL_DOCDIR})

