#include "socialproxy.h"
#include "socialplugin.h"
#include "socialmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>

SocialProxy::SocialProxy(QList<ISocialModule *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins), m_model(0)
{
    ISocialModule *plugin = m_plugins[0];
    connect(plugin, SIGNAL(authorizationStatusChanged(int)), SLOT(authenticated()));
    QWidget *w = plugin->authenticationWidget();
    w->show();
}

SocialProxy::~SocialProxy()
{
    delete m_model;
}

void SocialProxy::setModel(SocialModel *model)
{
    m_model = model;
}

SocialModel *SocialProxy::model() const
{
    return m_model;
}

void SocialProxy::authenticated()
{
    qDebug() << "desu desu";
    m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
}
