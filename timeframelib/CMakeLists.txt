# Copyright (c) ROSA Laboratory 2012
# License: GPLv3
#
# Authors:
# Evgeniy Auzhin <evgeniy.augin@osinit.ru>
# Julia Mineeva <julia.mineeva@osinit.ru>
# Alexander Skakov <alexander.skakov@osinit.ru>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 3,
#   or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the
#   Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

project(timeframelib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_timeframe_plugins_install_path}")

add_definitions(${QT_DEFINITIONS})
#add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DTIMEFRAMELIB_LIBRARY)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
qt4_wrap_cpp(MOC_OUT ${HEADERS})

add_library(timeframe SHARED ${HEADERS} ${SOURCES} ${MOC_OUT})
target_link_libraries(timeframe ${QT_LIBRARIES} )


install(TARGETS timeframe DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
