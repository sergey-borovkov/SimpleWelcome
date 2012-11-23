project(facebook-timeframe-plugin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_timeframe_plugins_install_path}")

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
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

add_library(facebook-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${MOC_OUT} ${QRC_SOURCES})
target_link_libraries(facebook-timeframe-plugin
                        ${KDE4_KDECORE_LIBS}
                        ${QJSON}
                        timeframe)

install(TARGETS facebook-timeframe-plugin DESTINATION "${_timeframe_plugins_install_path}")
