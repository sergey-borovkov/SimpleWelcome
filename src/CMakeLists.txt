qt4_automoc(${qtproject_SRCS})

set(RESOURCES timeframe/resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../timeframelib)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/local/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/local/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml ${CMAKE_CURRENT_SOURCE_DIR}/qml/timeframe/*.qml)

# Special target for QML files
add_custom_target(build-qml ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/qml" "${CMAKE_BINARY_DIR}/${_qml_install_path}"
    COMMENT "Building/copying the QML files"
)
kde4_add_executable(rosa-launcher ${HEADERS} ${SOURCES} ${QML} ${QRC_SOURCES})

find_package(Nepomuk REQUIRED)

SET(QT_USE_QDECLARATIVE TRUE)

target_link_libraries(rosa-launcher
                        ${KDE4_KDEUI_LIBS}
                        ${KDE4_PLASMA_LIBS}
                        ${KDE4_KIO_LIBS}
                        ${KDE4_KFILE_LIBS}
                        ${NEPOMUK_LIBRARIES}
                        ${NEPOMUK_QUERY_LIBRARIES}
                        ${SOPRANO_LIBRARIES}
                        timeframe)

add_custom_command(TARGET rosa-launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${_bin_dir_name}/${PROJECT_NAME}" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
)

# INSTALL
install(TARGETS rosa-launcher ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY "${CMAKE_BINARY_DIR}/${_qml_install_path}/" DESTINATION "${_qml_install_path}")
