project (facebook-timeframe-plugin)

find_package (Qt4 REQUIRED QtCore QtGui QtDeclarative QtNetwork)
include(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

find_package(QJSON REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/timeframe/social ${QT_QDECLARATIVE_INCLUDE_DIR})

set (RESOURCES resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml)
qt4_wrap_cpp(MOC_OUT ${HEADERS})

add_library(facebook-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${QML} ${MOC_OUT})
target_link_libraries(facebook-timeframe-plugin ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES} ${QJSON_LIBRARIES})

install(TARGETS facebook-timeframe-plugin DESTINATION share/timeframe)