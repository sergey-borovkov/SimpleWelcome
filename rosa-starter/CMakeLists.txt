project(rosa-starter)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES})

set(rosastarter_SRCS
    src/rosa-starter.cpp)

kde4_add_plugin(plasma_applet_rosastarter ${rosastarter_SRCS})
target_link_libraries(plasma_applet_rosastarter
        ${KDE4_PLASMA_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${KDE4_KIO_LIBS}
)

install(TARGETS plasma_applet_rosastarter
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-rosastarter.desktop DESTINATION ${SERVICES_INSTALL_DIR})
