#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "pluginitem.h"
#include "pluginmodel.h"
#include "../listmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

SocialProxy::SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent) :
    QObject(parent), m_plugins(plugins),
    m_socialModel(new ListModel(SocialItem::roleNames(), this)),
    m_pluginModel(new PluginModel(PluginItem::roleNames(), this)),
    m_anyEnabled(false)
{
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin *plugin, plugins)
    {
        QObject *object;
        if((object = dynamic_cast<QObject *>(plugin->requestManager())) != 0)
            connect(object, SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));

        if((object = dynamic_cast<QObject *>(plugin)) != 0)
            connect(object, SIGNAL(authorized()), SLOT(authorized()));

        PluginItem *item = new PluginItem(plugin);
        m_pluginModel->appendRow(item);

        // not the best solution... need to redo all this reading from settings later
        if(settings.value(plugin->name()).toInt() == 1)
        {
            plugin->requestManager()->queryWall(QDate(), QDate());
            m_anyEnabled = true;
        }
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

PluginModel *SocialProxy::pluginModel()
{
    return m_pluginModel;
}

void SocialProxy::startSearch()
{
    // not sure if I need to use this but for now whether plugins
    // are authorized will be kept here

    QSettings settings("ROSA", "Timeframe");
    foreach(ISocialPlugin *plugin, m_plugins)
    {
        if(settings.value(plugin->name()).toInt() == 1)
        {
            plugin->requestManager()->queryWall(QDate(), QDate());
        }
    }
}

void SocialProxy::authorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    plugin->requestManager()->queryWall(QDate(), QDate());
    if(plugin->authenticationWidget())
        plugin->authenticationWidget()->hide();
}

void SocialProxy::deauthorized()
{
}

bool SocialProxy::anyPluginsEnabled()
{
    return m_anyEnabled;
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
