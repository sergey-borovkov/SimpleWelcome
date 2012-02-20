project (rosa-launcher-qtquick)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

find_package (Qt4 REQUIRED QtCore QtGui QtDeclarative)
include(${QT_USE_FILE})
qt4_automoc(${qtproject_SRCS})
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories(${KDE4_INCLUDES} ${QT_QDECLARATIVE_INCLUDE_DIR})

set(rosa-launcher-qtquick_SRCS src/main.cpp src/swapp.cpp src/appentity.cpp src/appprovider.cpp src/appiconprovider.cpp src/qmlapplicationviewer/qmlapplicationviewer.cpp)

kde4_add_executable(rosa-launcher-qtquick ${rosa-launcher-qtquick_SRCS})
target_link_libraries(rosa-launcher-qtquick ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} kworkspace ${KDE4_KIO_LIBS} ${X11_LIBRARIES} ${KDE4_KFILE_LIBS} ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES})

install(TARGETS rosa-launcher-qtquick ${INSTALL_TARGETS_DEFAULT_ARGS})

file(GLOB qml_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.qml")
file(GLOB qml_js_files "${CMAKE_CURRENT_SOURCE_DIR}/src/qml/*.js")
install(FILES ${qml_files} DESTINATION share/${PROJECT_NAME}/qml)
install(FILES ${qml_js_files} DESTINATION share/${PROJECT_NAME}/qml)
