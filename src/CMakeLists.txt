# Copyright (c) ROSA Laboratory 2012
# License: GPLv3
#
# Authors:
# Sergey Borovkov <sergey.borovkov@osinit.ru>
# Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
# Alexander Skakov <alexander.skakov@osinit.ru>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 3,
#   or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the
#   Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

qt4_automoc(${qtproject_SRCS})

set(RESOURCES timeframe/resources.qrc)
qt4_add_resources(QRC_SOURCES ${RESOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../timeframelib)

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.h ${CMAKE_CURRENT_SOURCE_DIR}/shadowblur/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.h ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/local/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/qmlapplicationviewer/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/shadowblur/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/social/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/timeframe/local/*.cpp)
file(GLOB QML ${CMAKE_CURRENT_SOURCE_DIR}/qml/*.qml ${CMAKE_CURRENT_SOURCE_DIR}/qml/timeframe/*.qml ${CMAKE_CURRENT_SOURCE_DIR}/qml/timeframe/clouds/*.qml)

# Special target for QML files
add_custom_target(build-qml ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/qml" "${CMAKE_BINARY_DIR}/${_qml_install_path}"
    COMMENT "Building/copying the QML files"
)
kde4_add_executable(rosa-launcher ${HEADERS} ${SOURCES} ${QML} ${QRC_SOURCES})

find_package(Nepomuk REQUIRED)
find_package(KDeclarative REQUIRED)

SET(QT_USE_QDECLARATIVE TRUE)

target_link_libraries(rosa-launcher
                        ${KDE4_KDEUI_LIBS}
                        ${KDE4_PLASMA_LIBS}
                        ${KDE4_KIO_LIBS}
                        ${KDE4_KFILE_LIBS}
                        ${KDE4_PHONON_LIBS}
                        ${NEPOMUK_LIBRARIES}
                        ${NEPOMUK_QUERY_LIBRARIES}
                        ${SOPRANO_LIBRARIES}
                        ${KDECLARATIVE_LIBRARIES}
                        timeframe)

add_custom_command(TARGET rosa-launcher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${_bin_dir_name}/${PROJECT_NAME}" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
)

# INSTALL
install(TARGETS rosa-launcher ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY "${CMAKE_BINARY_DIR}/${_qml_install_path}/" DESTINATION "${_qml_install_path}")
