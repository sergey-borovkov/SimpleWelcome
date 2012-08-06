#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "socialdaymodel.h"
#include "pluginitem.h"
#include "pluginmodel.h"
#include "../listmodel.h"

#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

SocialProxy::SocialProxy( QList<ISocialPlugin *> plugins, QObject *parent )
    : QObject( parent )
    , m_plugins( plugins )
    , m_pluginModel( new PluginModel( PluginItem::roleNames(), this ) )
    , m_socialModel( 0 )
    , m_anyEnabled( false )
{
    QSettings settings("ROSA", "Timeframe");
    foreach(ISocialPlugin *plugin, plugins)
    {
        QObject *object = 0;
        if((object = dynamic_cast<QObject *>(plugin->requestManager())) != 0)
        {
            connect(object, SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));
        }
        if((object = dynamic_cast<QObject *>(plugin)) != 0)
        {
            connect(object, SIGNAL(authorized()), SLOT(authorized()));
            connect(object, SIGNAL(deauthorized()), SLOT(deauthorized()));
        }

        PluginItem *item = new PluginItem(plugin);
        m_pluginModel->appendRow(item);

        // not the best solution... need to redo all this reading from settings later
        if(settings.value(plugin->name()).toInt() == 1 && plugin->authorized() )
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
        qDebug() << plugin->name() << settings.value(plugin->name()).toInt();
        if(settings.value(plugin->name()).toInt() == 1)
            plugin->requestManager()->queryWall(QDate(), QDate());
    }
}

int SocialProxy::count() const
{
    return m_plugins.size();
}

QString SocialProxy::name(int i)
{
    return m_plugins.at(i)->name();
}

void SocialProxy::authorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    plugin->requestManager()->queryWall(QDate(), QDate());
    if ( plugin->authenticationWidget() )
        plugin->authenticationWidget()->hide();

    emit pluginAuthorized();
}

void SocialProxy::deauthorized()
{
    qDebug() << "SocialProxy::deauthorized";
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
    if (!m_socialModel)
        return;
    QList<SocialItem *> list;
    foreach( SocialItem *item, items )
    {
        QString strId = QString( "%1-%2" ).arg( item->pluginName() ).arg( item->id() );
        if ( m_idSet.contains( strId ) )
                continue;
        m_idSet.insert( strId );

        list.append( item );
        emit newMonth(item->date().year(), item->date().month(), item->pluginName());
    }
    m_socialModel->newSocialItems( list );
}

void SocialProxy::setModel(SocialDayModel *model)
{
    m_socialModel = model;
}
