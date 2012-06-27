#include "socialpluginmanager.h"
#include "socialplugin.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>

SocialPluginManager::SocialPluginManager(QList<ISocialModule *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins)
{
    ISocialModule *plugin = m_plugins[0];
    connect(plugin, SIGNAL(authorizationStatusChanged(int)), SLOT(authenticated()));
    QWidget *w = plugin->authenticationWidget();
    w->show();
}

void SocialPluginManager::authenticated()
{
    qDebug() << "desu desu";
    m_plugins[0]->requestManager()->queryWall(QDate(), QDate());
}
