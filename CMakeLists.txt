cmake_minimum_required(VERSION 2.8)

project (rosa-launcher-qtquick)

set(_implicit_install FALSE)

if(${CMAKE_INSTALL_PREFIX} STREQUAL NOT_SPECIFIED)
	# if CMAKE_INSTALL_PREFIX isn't specified we will implicit install project into binary dir (like make install) for
	# convenience of developers
	message("Warning: install prefix isn't specified, it will be set to ${CMAKE_BINARY_DIR}")
	set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
	set(_implicit_install TRUE)
endif()

configure_file("config.h.in" "config.h")
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(src)

install(DIRECTORY assets/ DESTINATION "share/${PROJECT_NAME}/assets")
