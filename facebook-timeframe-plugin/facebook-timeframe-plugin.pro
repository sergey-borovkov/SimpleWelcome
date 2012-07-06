TEMPLATE = lib
QT += declarative network
TARGET = facebook-timeframe-plugin
HEADERS += oauth2authorizer.h \
    requestmanager.h ../src/timeframe/social/socialplugin.h \
    request.h \
    facebookmodule.h \
    feeditem.h
SOURCES += oauth2authorizer.cpp \
    requestmanager.cpp \
    request.cpp \
    facebookmodule.cpp \
    feeditem.cpp
LIBS += -Lqjson/lib -lqjson
OTHER_FILES += \
    qml/main.qml

RESOURCES += \
    resources.qrc
DESTDIR = $$PWD/../build/lib/
target.path = $$PREFIX/share/timeframe/
INSTALLS = target
