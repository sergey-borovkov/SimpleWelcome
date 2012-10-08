cmake_minimum_required(VERSION 2.6)

project(rosa-launcher)

#set(CMAKE_MODULE_PATH /usr/share/kde4/apps/cmake/modules/)

# Some constants
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

find_package(Qt4 REQUIRED QtCore QtGui QtWebkit QtNetwork)
include(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES})

# Special target for assets
add_custom_target(build-assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/${_assets_install_path}"
    COMMENT "Building/copying the assets"
)

add_subdirectory(timeframelib)
add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(rosa-starter)
add_subdirectory(src)

install(DIRECTORY "${CMAKE_BINARY_DIR}/${_assets_install_path}/" DESTINATION "${_assets_install_path}")

add_subdirectory(po)
