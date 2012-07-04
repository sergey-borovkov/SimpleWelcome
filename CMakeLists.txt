project (rosa-launcher-qtquick)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

find_package (Qt4 REQUIRED QtCore QtGui QtDeclarative)
include(${QT_USE_FILE})
qt4_automoc(${qtproject_SRCS})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set (RESOURCES src/timeframe/resources.qrc)
QT4_ADD_RESOURCES(QRC_SOURCES ${RESOURCES})

ADD_SUBDIRECTORY(rosa-service-runner)

include_directories(${KDE4_INCLUDES} ${QT_QDECLARATIVE_INCLUDE_DIR})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/qmlapplicationviewer/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/timeframe/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/timeframe/social/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/qmlapplicationviewer/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/timeframe/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/timeframe/social/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml ${CMAKE_CURRENT_SOURCE_DIR}/src/qml/timeframe/*.qml)

kde4_add_executable(rosa-launcher-qtquick ${HEADERS} ${SOURCES} ${QML} ${QRC_SOURCES})

target_link_libraries(rosa-launcher-qtquick ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES} ${NEPOMUK_LIBRARIES} ${NEPOMUK_QUERY_LIBRARIES})


# INSTALL
install(TARGETS rosa-launcher-qtquick ${INSTALL_TARGETS_DEFAULT_ARGS})

file(GLOB qml_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml")
file(GLOB qml_tf_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/timeframe/*.qml")
file(GLOB qml_js_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.js")
file(GLOB asset_files "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.png")
install(FILES ${qml_files} DESTINATION share/${PROJECT_NAME}/qml)
install(FILES ${qml_tf_files} DESTINATION share/${PROJECT_NAME}/qml/timeframe/)
install(FILES ${qml_js_files} DESTINATION share/${PROJECT_NAME}/qml)
install(FILES ${asset_files} DESTINATION share/${PROJECT_NAME}/assets)

#add_subdirectory(facebook-timeframe-plugin)
