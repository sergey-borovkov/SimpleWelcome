#include "socialproxy.h"
#include "socialitem.h"
#include "socialplugin.h"
#include "socialdaymodel.h"
#include "pluginitem.h"
#include "pluginmodel.h"
#include "pluginrequestreply.h"

#include <listmodel.h>
#include <commentitem.h>

#include <QtGui/QWidget>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

SocialProxy::SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent)
    : QObject(parent)
    , m_plugins(plugins)
    , m_pluginModel(new PluginModel(PluginItem::roleNames(), this))
    , m_socialModel(0)
{
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin * plugin, plugins) {
        QObject *object = 0;
        if((object = dynamic_cast<QObject *>(plugin->requestManager())) != 0) {
            // perphaps need to make it work via PluginRequestReply later
            connect(object, SIGNAL(newSocialItems(QList<SocialItem*>)), SLOT(newItems(QList<SocialItem*>)));
        }
        if((object = dynamic_cast<QObject *>(plugin)) != 0) {
            connect(object, SIGNAL(authorized()), SLOT(authorized()));
            connect(object, SIGNAL(deauthorized()), SLOT(deauthorized()));
        }

        PluginItem *item = new PluginItem(plugin);
        m_pluginModel->appendRow(item);

        bool isEnabled = settings.value(plugin->name()).toBool();
        if(isEnabled && plugin->authorized()) {
            Request *requestId = plugin->requestManager()->queryUserId();
            requestId->start();
            Request *request = plugin->requestManager()->queryWall(QDate(), QDate());
            request->start();
            m_enabledPlugins.insert(plugin->name());
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

void SocialProxy::likeItem(const QString &id, const QString &pluginName)
{
    PluginRequestReply* reply = like(id, pluginName);
    connect(reply,SIGNAL(success(PluginRequestReply*)),this, SLOT(likeSuccess(PluginRequestReply*)));
    /*TO-DO: process error replies*/
    connect(reply,SIGNAL(finished()),reply,SLOT(deleteLater()));
}

void SocialProxy::dislikeItem(const QString &id, const QString &pluginName)
{
    PluginRequestReply* reply = dislike(id, pluginName);
    connect(reply,SIGNAL(success(PluginRequestReply*)),this, SLOT(likeSuccess(PluginRequestReply*)));
    /*TO-DO: process error replies*/
    connect(reply,SIGNAL(finished()),reply,SLOT(deleteLater()));
}

void SocialProxy::commentItem(const QString &message, const QString &parentId, const QString &pluginName)
{
    PluginRequestReply* reply = postComment(message, parentId, pluginName);
    m_cachedComment = message;
    connect(reply,SIGNAL(success(PluginRequestReply*)),this, SLOT(commentSuccess(PluginRequestReply*)));
    connect(reply,SIGNAL(finished()),reply,SLOT(deleteLater()));

}

void SocialProxy::startSearch()
{
    foreach(ISocialPlugin * plugin, m_plugins) {
        if(m_enabledPlugins.contains(plugin->name()))
            plugin->requestManager()->queryWall(QDate(), QDate());
    }
}

PluginRequestReply *SocialProxy::like(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    Request *request = plugin->requestManager()->like(id);
    PluginRequestReply *reply = new PluginRequestReply(request, id, this);
    request->start();

    return reply;
}

PluginRequestReply *SocialProxy::dislike(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    Request *request = plugin->requestManager()->dislike(id);
    PluginRequestReply *reply = new PluginRequestReply(request, id, this);
    request->start();
    return reply;
}

PluginRequestReply *SocialProxy::postComment(const QString &message, const QString &parentId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    Request *request = plugin->requestManager()->postComment(message, parentId);
    PluginRequestReply *reply = new PluginRequestReply(request, parentId, this);
    request->start();

    return reply;
}

int SocialProxy::authorizedPluginCount() const
{
    return m_enabledPlugins.count();
}

QString SocialProxy::authorizedPluginName(int i) const
{
    QStringList authorizedPlugins = m_enabledPlugins.toList();
    return authorizedPlugins.at(i);
}

void SocialProxy::likeSuccess(PluginRequestReply* reply)
{
    m_socialModel->likeItem(reply->sourceId());
}

void SocialProxy::commentSuccess(PluginRequestReply* reply)
{
    CommentItem* item = new CommentItem();
    item->setData(CommentItem::Message, m_cachedComment);
    item->setData(CommentItem::Id, reply->id());
    //TODO: set user id
    item->setData(CommentItem::From, "User");
    item->setData(CommentItem::FromId, "111");
    item->setData(CommentItem::CreatedTime, QDateTime::currentDateTime());
    m_socialModel->addCommentToItem(item, reply->sourceId());
}

void SocialProxy::authorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    m_enabledPlugins.insert(plugin->name());

    plugin->requestManager()->queryWall(QDate(), QDate());

    if(plugin->authenticationWidget())
        plugin->authenticationWidget()->hide();

    emit pluginAuthorized();
}

void SocialProxy::deauthorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    m_enabledPlugins.remove(plugin->name());

    // set plugin as disabled
    QSettings settings("ROSA", "Timeframe");
    settings.setValue(plugin->name(), 0);

    emit pluginDeauthorized();
}

bool SocialProxy::anyPluginsEnabled()
{
    return m_enabledPlugins.count() > 0;
}

void SocialProxy::newItem(SocialItem *item)
{
    newItems(QList<SocialItem *>() << item);
}

void SocialProxy::newItems(QList<SocialItem *> items)
{
    if(!m_socialModel)
        return;
    QList<SocialItem *> list;
    foreach(SocialItem * item, items) {
        QString strId = QString("%1-%2").arg(item->pluginName()).arg(item->id());
        if(m_idSet.contains(strId))
            continue;
        m_idSet.insert(strId);

        list.append(item);
        emit newMonth(item->date().year(), item->date().month(), item->pluginName());
    }
    m_socialModel->newSocialItems(list);
}

void SocialProxy::setModel(SocialDayModel *model)
{
    m_socialModel = model;
}

ISocialPlugin *SocialProxy::pluginFromName(const QString &pluginName)
{
    ISocialPlugin *plugin = 0;
    foreach (ISocialPlugin *p, m_plugins) {
        if(p->name() == pluginName) {
            plugin = p;
            break;
        }
    }
    return plugin;
}


