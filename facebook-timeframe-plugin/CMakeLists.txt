project (facebook-timeframe-plugin)

find_package (Qt4 REQUIRED QtCore QtGui QtNetwork QtWebkit)
include(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

find_package(QJSON REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/timeframe/social ${QT_QDECLARATIVE_INCLUDE_DIR})

set (RESOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
qt4_wrap_cpp(MOC_OUT ${HEADERS})

add_library(facebook-timeframe-plugin SHARED ${HEADERS} ${SOURCES} ${QML} ${MOC_OUT} ${QRC_SOURCES})
target_link_libraries(facebook-timeframe-plugin ${QT_LIBRARIES} ${QT_QDECLARATIVE_LIBRARIES} ${QT_WEBKIT_LIBRARIES} ${QJSON_LIBRARIES})

if(_implicit_install)
	get_target_property(_library_path facebook-timeframe-plugin LOCATION)
	add_custom_command(TARGET facebook-timeframe-plugin POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/${_timeframe_plugins_install_path}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_library_path}" "${CMAKE_INSTALL_PREFIX}/${_timeframe_plugins_install_path}/"
	)
endif()

install(TARGETS facebook-timeframe-plugin DESTINATION "${_timeframe_plugins_install_path}")
