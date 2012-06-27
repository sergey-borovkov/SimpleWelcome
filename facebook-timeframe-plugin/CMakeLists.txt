project (facebook-timeframe-plugin)

include(${QT_USE_FILE})
qt4_automoc(${qtproject_SRCS})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/timeframe/social ${QT_QDECLARATIVE_INCLUDE_DIR})

set (RESOURCES resources.qrc)
QT4_ADD_RESOURCES(QRC_SOURCES ${RESOURCES})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h "${CMAKE_CURRENT_SOURCE_DIR}/../src/timeframe/social/socialplugin.h")
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)
qt4_wrap_cpp (MOC_OUTFILES ${HEADERS})

ADD_LIBRARY(facebook-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${QML} ${MOC_OUTFILES})
target_link_libraries(facebook-timeframe-plugin ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES})
