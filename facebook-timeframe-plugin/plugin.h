#ifndef PLUGIN1_H
#define PLUGIN1_H

#include "../src/timeframe/social/socialplugin.h"

#include <QtPlugin>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QGraphicsObject>

class FacebookModule : public QObject, public ISocialModule
{
    Q_OBJECT
    Q_INTERFACES(ISocialModule)
public:
    FacebookModule()
    {
        authorizationView = new QDeclarativeView(QUrl("qrc:/qml/main.qml"));
    }

    ~FacebookModule() {}
    QWidget *authenticationWidget()
    {        
        return authorizationView;
    }

private:
    QDeclarativeView *authorizationView;
};

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
QT_END_NAMESPACE

#endif
