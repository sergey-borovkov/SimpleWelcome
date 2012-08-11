find_package(KDE4 REQUIRED)
include (KDE4Defaults)

find_package (Qt4 REQUIRED QtCore QtGui QtDeclarative)
include(${QT_USE_FILE})
qt4_automoc(${qtproject_SRCS})
add_definitions(${QT_DEFINITIONS})
set (RESOURCES timeframe/resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

include_directories(${KDE4_INCLUDES} ${QT_QDECLARATIVE_INCLUDE_DIR})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml ${CMAKE_CURRENT_SOURCE_DIR}/qml/timeframe/*.qml)

kde4_add_executable(rosa-launcher-qtquick ${HEADERS} ${SOURCES} ${QML} ${QRC_SOURCES})

target_link_libraries(rosa-launcher-qtquick ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES} ${NEPOMUK_LIBRARIES} ${NEPOMUK_QUERY_LIBRARIES})


if(_implicit_install)
	get_target_property(_executable_path rosa-launcher-qtquick LOCATION)
	add_custom_command(TARGET rosa-launcher-qtquick POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/${_bin_dir_name}"
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/${_share_dir_name}"
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/${_timeframe_plugins_install_path}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_executable_path}" "${CMAKE_INSTALL_PREFIX}/${_bin_dir_name}/"
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_INSTALL_PREFIX}/${_assets_install_path}"
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/qml" "${CMAKE_INSTALL_PREFIX}/${_qml_install_path}"
		COMMAND ${CMAKE_COMMAND} -E create_symlink "${_bin_dir_name}/${PROJECT_NAME}" "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}"
	)
endif()


# INSTALL
install(TARGETS rosa-launcher-qtquick ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY qml/ DESTINATION "${_qml_install_path}")
