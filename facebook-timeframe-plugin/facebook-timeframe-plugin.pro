TEMPLATE = lib
QT += declarative network
TARGET = facebook-timeframe-plugin
HEADERS += plugin.h oauth2authorizer.h facebookmanager.h \
    requestmanager.h
SOURCES += plugin.cpp oauth2authorizer.cpp facebookmanager.cpp \
    requestmanager.cpp

OTHER_FILES += \
    qml/main.qml

RESOURCES += \
    resources.qrc
DESTDIR = $$PWD/../build/lib/
