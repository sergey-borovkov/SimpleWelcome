# Copyright (c) ROSA Laboratory 2012
# License: GPLv3
#
# Authors:
# Julia Mineeva <julia.mineeva@osinit.ru>
# Sergey Borovkov <sergey.borovkov@osinit.ru>
# Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
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

project(vkontakte-timeframe-plugin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_timeframe_plugins_install_path}")

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

find_library(QJSON qjson)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../timeframelib)

set (RESOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
qt4_wrap_cpp(MOC_OUT ${HEADERS})

set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTWEBKIT TRUE)

add_library(vkontakte-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${MOC_OUT} ${QRC_SOURCES})
target_link_libraries(vkontakte-timeframe-plugin
                        ${KDE4_KDECORE_LIBS}
                        ${QJSON}
                        timeframe)

install(TARGETS vkontakte-timeframe-plugin DESTINATION "${_timeframe_plugins_install_path}")
