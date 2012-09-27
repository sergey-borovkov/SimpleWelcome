project(timeframelib)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${_timeframe_plugins_install_path}")

find_package(Qt4 REQUIRED QtCore)
include(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DTIMEFRAMELIB_LIBRARY)

include_directories( ${QT_QDECLARATIVE_INCLUDE_DIR})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
qt4_wrap_cpp(MOC_OUT ${HEADERS})

add_library(timeframe SHARED ${HEADERS} ${SOURCES} ${MOC_OUT})
target_link_libraries(timeframe ${QT_LIBRARIES} )


install(TARGETS timeframe DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
