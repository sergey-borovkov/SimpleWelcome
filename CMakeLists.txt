# Copyright (c) ROSA Laboratory 2012
# License: GPLv3
#
# Authors:
# Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
# Sergey Borovkov <sergey.borovkov@osinit.ru>
# Alexander Skakov <alexander.skakov@osinit.ru>
# Julia Mineeva <julia.mineeva@osinit.ru>
# Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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

cmake_minimum_required(VERSION 2.6)

project(rosa-launcher)

#set(CMAKE_MODULE_PATH /usr/share/kde4/apps/cmake/modules/)

# Some constants
set(APPLICATION_NAME "ROSA_Launcher" CACHE STRING "A name of application (it isn't a name of executable file)")
set(_bin_dir_name "bin")
set(_share_dir_name "share") # name of system share directory
set(_assets_dir_path "assets") # path to assets directory from share directory
set(_qml_dir_path "qml") # name of directory for qml
set(_timeframe_plugins_path "timeframe") #path to timeframe plugins from share directory
set(_share_dir_path "${PROJECT_NAME}") # path to main share directory of application from system share directory
set(_share_dir_install_path "${_share_dir_name}/${_share_dir_path}") # path where share files have to be installed to
set(_timeframe_plugins_install_path "${_share_dir_name}/${_timeframe_plugins_path}") # path where plugins have to be installed to
set(_assets_install_path "${_share_dir_install_path}/${_assets_dir_path}") # path where assets have to be installed to
set(_qml_install_path "${_share_dir_install_path}/${_qml_dir_path}") # path where qml files have to be installed to
set(pot_file "${CMAKE_BINARY_DIR}/${APPLICATION_NAME}.pot")

# the library install dir
set(LIBRARY_INSTALL_DIR "${_DEFAULT_LIBRARY_INSTALL_DIR}" CACHE PATH "Installation directory for libraries")
# make the library install dir an absolute path (can be important e.g. when using CONFIGURE_FILE to embed
# the library installation directory into a file)
if(NOT IS_ABSOLUTE "${LIBRARY_INSTALL_DIR}")
  set(LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIR}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_bin_dir_name}")

#check out of source build
get_filename_component(_src_dir_real_path "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(_bin_dir_real_path "${CMAKE_BINARY_DIR}" REALPATH)

if(${_src_dir_real_path} STREQUAL ${_bin_dir_real_path})
    message(FATAL_ERROR "In-source builds are forbidden! Choose separate build directory.")
endif()

configure_file("config.h.in" "config.h")
include_directories(${CMAKE_BINARY_DIR})

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

find_package(Qt4 REQUIRED QtCore QtGui QtWebkit QtNetwork QtDeclarative)
include(${QT_USE_FILE})

add_definitions(${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES})

# Special target for assets
add_custom_target(build-assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/${_assets_install_path}"
    COMMENT "Building/copying the assets"
)

# Special target for creating pot
find_program(XGETTEXT_EXECUTABLE xgettext DOC "Path to xgettext program")
if(XGETTEXT_EXECUTABLE)
    message(STATUS "Found xgettext: ${XGETTEXT_EXECUTABLE}")
    message(STATUS "Target 'build-pot' will be available for creating pot file")

    set(filelist "${CMAKE_BINARY_DIR}/all_src_filelist")

    # collect names of files
    set(dirs "src" "rosa-starter" "timeframelib" "facebook-timeframe-plugin" "vkontakte-timeframe-plugin")
    file(WRITE ${filelist} "")
    foreach(d ${dirs})
        file(GLOB_RECURSE files_src RELATIVE ${CMAKE_SOURCE_DIR} "${d}/*.cpp" "${d}/*.h" "${d}/*.c" "${d}/*.qml" "${d}/*.js")
        foreach(f ${files_src})
            file(APPEND ${filelist} "${f}\n")
        endforeach(f)
    endforeach(d)

    # generate pot file
    add_custom_target(build-pot
        COMMAND ${XGETTEXT_EXECUTABLE} -from-code=UTF-8 -C -kde -ci18n -ki18n:1 -ki18nc:1c,2
            -ki18np:1,2 -ki18ncp:1c,2,3 -ktr2i18n:1 -kI18N_NOOP:1 -kI18N_NOOP2:1c,2 -kaliasLocale
            -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3
            --files-from=${filelist} -D ${CMAKE_SOURCE_DIR} -o ${pot_file}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Building pot file"
        VERBATIM
    )
endif()


add_subdirectory(timeframelib)
add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(rosa-starter)
add_subdirectory(src)

install(DIRECTORY "${CMAKE_BINARY_DIR}/${_assets_install_path}/" DESTINATION "${_assets_install_path}")

add_subdirectory(po)

# Deb-package generation part
set(CPACK_GENERATOR "DEB")
set(CPACK_SET_DESTDIR "on")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")

set(CPACK_PACKAGE_DESCRIPTION "SimpleWelcome")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pretty application launcher for KDE")
set(CPACK_PACKAGE_CONTACT "Alexander Skakov <zoinen@gmail.com>")

set(${VERSION} CPACK_DEBIAN_PACKAGE_VERSION)
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION "system")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "kdebase-workspace-dev, libqjson-dev")

set(CPACK_COMPONENTS_ALL Libraries ApplicationData)
include(CPack Documentation)
