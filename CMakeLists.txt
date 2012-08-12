cmake_minimum_required(VERSION 2.8)

project (rosa-launcher-qtquick)

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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_bin_dir_name}")

configure_file("config.h.in" "config.h")
include_directories(${CMAKE_BINARY_DIR})

# Special target for assets
add_custom_target(build-assets ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/${_assets_install_path}"
	COMMENT "Building/copying the assets"
)

add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(src)

install(DIRECTORY "${CMAKE_BINARY_DIR}/${_assets_install_path}/" DESTINATION "${_assets_install_path}")
