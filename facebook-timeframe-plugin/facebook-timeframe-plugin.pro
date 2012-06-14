TEMPLATE = lib
QT += declarative network
TARGET = facebook-timeframe-plugin
HEADERS += plugin.h oauth2authorizer.h facebookmanager.h
SOURCES += plugin.cpp oauth2authorizer.cpp facebookmanager.cpp

OTHER_FILES += \
    qml/main.qml \
    qml/FacebookAuthWeb.qml \
    qml/FacebookStatusUpdatesView.qml \
    qml/ServiceHeader.qml

RESOURCES += \
    resources.qrc
DESTDIR = $$PWD/../build/lib/
