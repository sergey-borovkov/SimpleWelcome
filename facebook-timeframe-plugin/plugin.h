#ifndef PLUGIN1_H
#define PLUGIN1_H

#include "../src/timeframe/social/socialplugin.h"
#include "oauth2authorizer.h"
#include "facebookmanager.h"

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
    {}
    ~FacebookModule() {}
    QWidget *authenticationWidget()
    {
        QDeclarativeView *authorizationView = new QDeclarativeView(QUrl("qrc:/qml/main.qml"));
        OAuth2Authorizer authorizer;
        //authorizationView.rootContext()->setContextProperty("authorizer", &authorizer);

/*        QObject *rootItem = authorizationView->rootObject();
        QObject::connect(rootItem, SIGNAL(loadStarted(QUrl)),
                         &authorizer, SLOT(onWebLoadStarted(QUrl)));
    //    QObject::connect(rootItem, SIGNAL(loadFinished(QUrl)),
    //                     &authorizer, SLOT(onWebLoadFinished(QUrl)));

        FacebookManager fManager;
        fManager.setRootContext(authorizationView.rootContext());
        QObject::connect(&authorizer, SIGNAL(accessTokenChanged(QString)),
                         &fManager, SLOT(onAccessTokenChanged(QString)));
*/
        return authorizationView;
    }
};

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(facebook-timeframe-plugin, FacebookModule)
QT_END_NAMESPACE

#endif
