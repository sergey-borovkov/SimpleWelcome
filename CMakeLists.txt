cmake_minimum_required(VERSION 2.8)

project (rosa-launcher-qtquick)

set(_share_dir "../share/${PROJECT_NAME}") # path to main share directory

configure_file("config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(src)

install(DIRECTORY assets/ DESTINATION "share/${PROJECT_NAME}/assets")
