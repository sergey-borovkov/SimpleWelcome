# Copyright (c) ROSA Laboratory 2012
# License: GPLv3
#
# Authors:
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

project(rosa-starter)

set(rosastarter_SRCS
    src/rosa-starter.cpp)

include_directories(${CMAKE_BINARY_DIR})

kde4_add_plugin(plasma_applet_rosastarter ${rosastarter_SRCS})
target_link_libraries(plasma_applet_rosastarter
        ${KDE4_PLASMA_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${KDE4_KIO_LIBS}
)

install(TARGETS plasma_applet_rosastarter
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-rosastarter.desktop DESTINATION ${SERVICES_INSTALL_DIR})
