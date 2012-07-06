project (facebook-timeframe-plugin)

find_package (Qt4 REQUIRED QtCore QtGui QtDeclarative QtNetwork)
include(${QT_USE_FILE})
qt4_automoc(${qtproject_SRCS})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

find_package(QJSON REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/timeframe/social ${QT_QDECLARATIVE_INCLUDE_DIR})

set (RESOURCES resources.qrc)
QT4_ADD_RESOURCES(QRC_SOURCES ${RESOURCES})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)

ADD_LIBRARY(facebook-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${QML})
target_link_libraries(facebook-timeframe-plugin ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES} ${QJSON_LIBRARIES})
