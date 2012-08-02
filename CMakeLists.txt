cmake_minimum_required(VERSION 2.8)

project (rosa-launcher-qtquick)

add_subdirectory(rosa-service-runner)
#add_subdirectory(facebook-timeframe-plugin)
add_subdirectory(vkontakte-timeframe-plugin)
add_subdirectory(src)

file(GLOB asset_files "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.png")
install(FILES ${asset_files} DESTINATION share/${PROJECT_NAME}/assets)
