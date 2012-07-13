#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "pluginitem.h"
#include "../listmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>

SocialProxy::SocialProxy(QList<ISocialModule *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins),
    m_socialModel(new ListModel(SocialItem::roleNames(), this)),
    m_pluginModel(new ListModel(PluginItem::roleNames(), this))
{
    foreach(ISocialModule *plugin, plugins)
    {
        QObject *object;
        if((object = dynamic_cast<QObject *>(plugin->requestManager())) != 0)
            connect(object, SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));

        if((object = dynamic_cast<QObject *>(plugin)) != 0)
            connect(object, SIGNAL(authorized()), SLOT(authorized()));

        PluginItem *item = new PluginItem(plugin);
        m_pluginModel->appendRow(item);

        plugin->requestManager()->queryWall(QDate(), QDate());
    }
}

SocialProxy::~SocialProxy()
{
    qDeleteAll(m_plugins);
}

ListModel *SocialProxy::socialModel()
{
    return m_socialModel;
}

ListModel *SocialProxy::pluginModel()
{
    return m_pluginModel;
}

void SocialProxy::authorized()
{
    ISocialModule *plugin = dynamic_cast<ISocialModule *>(sender());
    plugin->requestManager()->queryWall(QDate(), QDate());
    if(plugin->authenticationWidget())
        plugin->authenticationWidget()->hide();
}

void SocialProxy::deauthorized()
{
}

void SocialProxy::newItem(SocialItem *item)
{
    newItems(QList<SocialItem *>() << item);
}

void SocialProxy::newItems(QList<SocialItem *> items)
{
    QList<ListItem *> list;
    foreach(SocialItem *item, items)
        list.append(item);
    m_socialModel->appendRows(list);
}
