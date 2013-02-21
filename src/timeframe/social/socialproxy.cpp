/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeva@osinit.ru>
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "pluginitem.h"
#include "pluginmodel.h"
#include "pluginreply.h"
#include "requestqueue.h"
#include "socialcontentmodel.h"
#include "socialitem.h"
#include "socialproxy.h"

#include <commentitem.h>
#include <listmodel.h>
#include <requestqueue.h>
#include <socialplugin.h>

#include <QtCore/QDate>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtGui/QWidget>

SocialProxy::SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent)
    : QObject(parent)
    , m_plugins(plugins)
    , m_pluginModel(new PluginModel(PluginItem::roleNames(), this))
    , m_socialModel(0)
    , m_searchInProgressCount(0)
{
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin * plugin, plugins) {
        QObject *object = 0;
        if ((object = dynamic_cast<QObject *>(plugin->requestManager())) != 0) {
            // perphaps need to make it work via PluginRequestReply later
            connect(object, SIGNAL(newSocialItems(QList<SocialItem *>)), SLOT(newItems(QList<SocialItem *>)));
            connect(object, SIGNAL(selfId(QString)), SLOT(onSelfId(QString)));
            connect(object, SIGNAL(selfName(QString)), SLOT(onSelfName(QString)));
            connect(object, SIGNAL(newComments(QString, QList<CommentItem *>)), SLOT(newComments(QString, QList<CommentItem *>)));
            connect(object, SIGNAL(searchComplete()), SLOT(searchComplete()));
            connect(object, SIGNAL(selfLiked(QString)), SLOT(onSelfLiked(QString)));
        }
        if ((object = dynamic_cast<QObject *>(plugin)) != 0) {
            connect(object, SIGNAL(authorized()), SLOT(authorized()));
            connect(object, SIGNAL(deauthorized()), SLOT(deauthorized()));
        }

        PluginItem *item = new PluginItem(plugin);
        m_pluginModel->appendRow(item);

        bool isEnabled = settings.value(plugin->name()).toBool();
        if (isEnabled && plugin->authorized()) {
            m_enabledPlugins.insert(plugin->name());
        }

        RequestQueue *requestQueue = new RequestQueue(parent);
        plugin->setRequestQueue(requestQueue);
        m_requestQueueMap.insert(plugin->name(), requestQueue);
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
    PluginReply *reply = like(id, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(likeSuccess(PluginReply *)));
    /*TO-DO: process error replies*/
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void SocialProxy::unlikeItem(const QString &id, const QString &pluginName)
{
    PluginReply *reply = dislike(id, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(likeSuccess(PluginReply *)));
    /*TO-DO: process error replies*/
}

void SocialProxy::logout(const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (plugin && plugin->authorized()) {
        plugin->requestManager()->logout();
        emit removeType(pluginName);
        m_socialModel->removeItems(pluginName);
    }
}

void SocialProxy::login(const QString &pluginName)
{
    // add this plugin to list of enabled plugins
    // should move after logout
    ISocialPlugin *plugin = pluginFromName(pluginName);
    QSettings settings("ROSA", "Timeframe");
    settings.setValue(plugin->name(), 1);

    if (plugin) {
        QWidget *w = plugin->authenticationWidget();
        if (w)
            w->show();
    }
}

void SocialProxy::commentItem(const QString &message, const QString &parentId, const QString &pluginName)
{
    PluginReply *reply = postComment(message, parentId, pluginName);
    m_cachedComment = message;
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(commentSuccess(PluginReply *)));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void SocialProxy::postMessage(const QString &message, const QString &pluginName)
{
    PluginReply *reply = postToWall(message, pluginName);
    m_cachedComment = message;
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(postToWallSuccess(PluginReply *)));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void SocialProxy::getUserPicture(const QString &id, const QString &parentId, const QString &pluginName)
{
    PluginReply *reply = userPicture(id, parentId, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(getPictureSuccess(PluginReply *)));
    /*TO-DO: process error replies*/
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void SocialProxy::startSearch()
{
    bool start = false;
    foreach(ISocialPlugin * plugin, m_plugins) {
        if (m_enabledPlugins.contains(plugin->name())) {
            Request *requestId = plugin->requestManager()->queryUserId();
            m_requestQueueMap[plugin->name()]->enqueue(requestId, Request::High);
            Request *request = plugin->requestManager()->queryWall();
            m_searchInProgressCount++;
            m_requestQueueMap[plugin->name()]->enqueue(request, Request::High);
            start = true;
        }
    }
    if (start)
        emit searchStarted();
}

PluginReply *SocialProxy::like(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;
    Request *request = plugin->requestManager()->like(id);
    PluginReply *reply = new PluginReply(request, id, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

PluginReply *SocialProxy::dislike(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;
    Request *request = plugin->requestManager()->unlike(id);
    PluginReply *reply = new PluginReply(request, id, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

PluginReply *SocialProxy::postComment(const QString &message, const QString &parentId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->postComment(QUrl::toPercentEncoding(message), parentId);
    PluginReply *reply = new PluginReply(request, parentId, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

PluginReply *SocialProxy::postToWall(const QString &message, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->postToWall(QUrl::toPercentEncoding(message));
    PluginReply *reply = new PluginReply(request, 0, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

PluginReply *SocialProxy::userPicture(const QString &id, const QString &parentId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    Request *request = plugin->requestManager()->queryImage(id);
    PluginReply *reply = new PluginReply(request, parentId, pluginName, this);
    QObject *obj = dynamic_cast<QObject *>(plugin->requestManager());
    connect(obj, SIGNAL(gotUserImage(QString, QString)), reply, SLOT(gotUserPictureUrl(QString, QString)));
    connect(obj, SIGNAL(gotUserName(QString, QString)), reply, SLOT(gotUserName(QString, QString)));
    m_requestQueueMap[plugin->name()]->enqueue(request);

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

QString SocialProxy::authorizedLocalizedPluginName(int i) const
{
    QStringList authorizedPlugins = m_enabledPlugins.toList();

    QString name = authorizedPlugins.at(i);
    QString displayName = name;
    foreach(ISocialPlugin * plugin, m_plugins) {
        if (plugin->name() == name) {
            displayName = plugin->displayName();
        }
    }

    return displayName;
}

PluginReply *SocialProxy::getAllComments(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;
    Request *request = plugin->requestManager()->queryComments(id);
    PluginReply *reply = new PluginReply(request, id, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request, Request::High);
    return reply;
}

PluginReply *SocialProxy::getAllLikes(const QString &id, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    Request *request = plugin->requestManager()->queryLikes(id);
    if (request == 0)
        return 0;
    PluginReply *reply = new PluginReply(request, id, pluginName, this);
    m_requestQueueMap[plugin->name()]->enqueue(request);
    return reply;
}

void SocialProxy::likeSuccess(PluginReply *reply)
{
    m_socialModel->likeItem(reply->sourceId());
}

void SocialProxy::commentSuccess(PluginReply *reply)
{
    ISocialPlugin *plugin = pluginFromName(reply->pluginName());

    CommentItem *item = new CommentItem();
    item->setData(CommentItem::Message, m_cachedComment);
    item->setData(CommentItem::Id, reply->id());
    item->setData(CommentItem::From, plugin ? plugin->selfName() : "");
    item->setData(CommentItem::FromId, plugin ? plugin->selfId() : "");
    item->setData(CommentItem::FromPictureUrl, plugin ? plugin->selfPictureUrl() : "images/user.png");
    item->setData(CommentItem::CreatedTime, QDateTime::currentDateTime());
    m_socialModel->addCommentToItem(item, reply->sourceId());
}

void SocialProxy::postToWallSuccess(PluginReply *reply)
{
    ISocialPlugin *plugin = pluginFromName(reply->pluginName());

    // NEED ADD MESSAGE TO MODEL!!!!

    SocialItem *item = new SocialItem(plugin->selfId());
    item->setId(reply->id());
    item->setData(SocialItem::Id, reply->id());
    item->setData(SocialItem::FromId, plugin->selfId());
    item->setData(SocialItem::FromName, plugin->selfName());
    item->setData(SocialItem::Text, m_cachedComment);
    item->setData(SocialItem::DateTime, QDateTime::currentDateTime());
    item->setData(SocialItem::ImageUrl, QUrl(""));
    item->setData(SocialItem::FromImageUrl, plugin->selfPictureUrl());
    item->setData(SocialItem::PluginName, reply->pluginName());
    item->setData(SocialItem::Like, false);
    item->setData(SocialItem::Likes, 0);
    item->setData(SocialItem::CommentCount, 0);

    newItem(item);
}

void SocialProxy::getPictureSuccess(PluginReply *reply)
{
    m_socialModel->updateUserImage(reply->userId(), reply->userPictureUrl(), reply->sourceId());
    if (!reply->userName().isEmpty()) {
        m_socialModel->updateUserName(reply->userId(), reply->userName(), reply->sourceId());
    }
}

void SocialProxy::getSelfPictureSuccess(PluginReply *reply)
{
    Q_UNUSED(reply)
}

void SocialProxy::authorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    m_enabledPlugins.insert(plugin->name());

    // update self id
    Request *requestId = plugin->requestManager()->queryUserId();
    m_requestQueueMap[plugin->name()]->enqueue(requestId, Request::High);

    // update wall
    Request *request = plugin->requestManager()->queryWall();
    m_searchInProgressCount++;
    m_requestQueueMap[plugin->name()]->enqueue(request, Request::High);

    emit pluginAuthorized();
    emit searchStarted();
}

void SocialProxy::searchComplete()
{
    m_searchInProgressCount--;
    if (!m_searchInProgressCount) {
        emit searchFinished();
    }
}

void SocialProxy::deauthorized()
{
    ISocialPlugin *plugin = dynamic_cast<ISocialPlugin *>(sender());
    m_enabledPlugins.remove(plugin->name());

    // set plugin as disabled
    QSettings settings("ROSA", "Timeframe");
    settings.setValue(plugin->name(), 0);

    plugin->setSelfId("");
    plugin->setSelfName("");
    plugin->setSelfPictureUrl("");

    emit pluginDeauthorized();
}

bool SocialProxy::anyPluginsEnabled()
{
    return m_enabledPlugins.count() > 0;
}

QString SocialProxy::selfId(const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (plugin) {
        QSettings settings("ROSA", "Timeframe");
        bool isEnabled = settings.value(pluginName).toBool();
        if (isEnabled && plugin->authorized()) {
            return plugin->selfId();
        }
    }

    return QString("");
}

QString SocialProxy::selfName(const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (plugin) {
        QSettings settings("ROSA", "Timeframe");
        bool isEnabled = settings.value(pluginName).toBool();
        if (isEnabled && plugin->authorized()) {
            return plugin->selfName();
        }
    }

    return QString("");
}

QString SocialProxy::selfPictureUrl(const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (plugin) {
        QSettings settings("ROSA", "Timeframe");
        bool isEnabled = settings.value(pluginName).toBool();
        if (isEnabled && plugin->authorized()) {
            return plugin->selfPictureUrl();
        }
    }

    return QString("images/user.png");
}

void SocialProxy::newItem(SocialItem *item)
{
    newItems(QList<SocialItem *>() << item);
}

void SocialProxy::newItems(QList<SocialItem *> items)
{
    if (!m_socialModel)
        return;

    ISocialRequestManager *manager = dynamic_cast<ISocialRequestManager *>(sender());
    QSettings settings("ROSA", "Timeframe");

    foreach(SocialItem * item, items) {
        emit newMonth(item->datetime().date().year(), item->datetime().date().month(), item->data(SocialItem::PluginName).toString());

        foreach(ISocialPlugin * plugin, m_plugins) {
            bool isEnabled = settings.value(plugin->name()).toBool();
            if (isEnabled && plugin->authorized() && manager && (plugin->requestManager() == manager)) {
                // get audio url
                if (!item->data(SocialItem::AudioId).toString().isEmpty() && !item->data(SocialItem::AudioOwnerId).toString().isEmpty())
                    getAudio(item->data(SocialItem::Id).toString(), item->data(SocialItem::AudioId).toString(), item->data(SocialItem::AudioOwnerId).toString(), plugin->name());
                // get video url
                if (!item->data(SocialItem::VideoId).toString().isEmpty() && !item->data(SocialItem::VideoOwnerId).toString().isEmpty())
                    getVideo(item->data(SocialItem::Id).toString(), item->data(SocialItem::VideoId).toString(), item->data(SocialItem::VideoOwnerId).toString(), plugin->name());

                if (!item->data(SocialItem::FromId).toString().isEmpty()) {
                    getUserInfo(item->data(SocialItem::Id).toString(), item->data(SocialItem::FromId).toString(), plugin->name());
                }
            }
        }
    }
    m_socialModel->newSocialItems(items);
}

PluginReply *SocialProxy::selfPicture(const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->queryImage(plugin->selfId());
    PluginReply *reply = new PluginReply(request, 0, pluginName, this);
    QObject *obj = dynamic_cast<QObject *>(plugin->requestManager());
    connect(obj, SIGNAL(gotUserImage(QString, QString)), SLOT(gotUserPictureUrl(QString, QString)));
    m_requestQueueMap[pluginName]->enqueue(request);

    return reply;
}

void SocialProxy::getSelfUserPicture(const QString &pluginName)
{
    PluginReply *reply = selfPicture(pluginName);
//    connect(reply, SIGNAL(success(PluginRequestReply*)), this, SLOT(getSelfPictureSuccess(PluginRequestReply*)));
    /*TO-DO: process error replies*/
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

PluginReply *SocialProxy::audioUrl(const QString &parentId, const QString &aid, const QString &ownerId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->queryAudio(aid, ownerId);
    PluginReply *reply = new PluginReply(request, parentId, pluginName, this);
    QObject *obj = dynamic_cast<QObject *>(plugin->requestManager());
    connect(obj, SIGNAL(gotAudioUrl(QString, QString, QString)), reply, SLOT(gotAudioUrl(QString, QString, QString)));

    if (request != 0)
        m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

void SocialProxy::getAudioSuccess(PluginReply *reply)
{
    m_socialModel->updateAudioUrl(reply->audioId(), reply->audioOwnerId(), reply->audioUrl(), reply->sourceId());
}

void SocialProxy::getAudio(const QString &parentId, const QString &aid, const QString &ownerId, const QString &pluginName)
{
    PluginReply *reply = audioUrl(parentId, aid, ownerId, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(getAudioSuccess(PluginReply *)));
    /*TO-DO: process error replies*/
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

PluginReply *SocialProxy::videoUrl(const QString &parentId, const QString &vid, const QString &ownerId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->queryVideo(vid, ownerId);
    PluginReply *reply = new PluginReply(request, parentId, pluginName, this);
    QObject *obj = dynamic_cast<QObject*>(plugin->requestManager());
    connect(obj, SIGNAL(gotVideoUrl(QString, QString, QString, QString)), reply, SLOT(gotVideoUrl(QString, QString, QString, QString)));

    if (request != 0)
        m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

void SocialProxy::getVideoSuccess(PluginReply *reply)
{
    m_socialModel->updateVideoUrl(reply->videoId(), reply->videoOwnerId(), reply->videoUrl(), reply->videoImage(), reply->sourceId());
}

void SocialProxy::getVideo(const QString &parentId, const QString &vid, const QString &ownerId, const QString &pluginName)
{
    PluginReply *reply = videoUrl(parentId, vid, ownerId, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(getVideoSuccess(PluginReply *)));
    /*TO-DO: process error replies*/
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

PluginReply *SocialProxy::userInfo(const QString &parentId, const QString &fromId, const QString &pluginName)
{
    ISocialPlugin *plugin = pluginFromName(pluginName);
    if (!plugin)
        return 0;

    Request *request = plugin->requestManager()->queryUserInfo(fromId);
    PluginReply *reply = new PluginReply(request, parentId, pluginName, this);
    QObject *obj = dynamic_cast<QObject*>(plugin->requestManager());
    connect(obj, SIGNAL(gotUserInfo(QString, QString, QString)), reply, SLOT(gotUserInfo(QString, QString, QString)));

    m_requestQueueMap[plugin->name()]->enqueue(request);

    return reply;
}

void SocialProxy::getUserInfoSuccess(PluginReply *reply)
{
    m_socialModel->updateUserInfo(reply->userName(), reply->userPictureUrl(), reply->sourceId());
}

void SocialProxy::getUserInfo(const QString &parentId, const QString &fromId, const QString &pluginName)
{
    PluginReply *reply = userInfo(parentId, fromId, pluginName);
    connect(reply, SIGNAL(success(PluginReply *)), this, SLOT(getUserInfoSuccess(PluginReply *)));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
}

void SocialProxy::onSelfId(QString id)
{
    ISocialRequestManager *manager = dynamic_cast<ISocialRequestManager *>(sender());
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin * plugin, m_plugins) {
        bool isEnabled = settings.value(plugin->name()).toBool();
        if (isEnabled && plugin->authorized() && manager && (plugin->requestManager() == manager)) {
            plugin->setSelfId(id);
            // get self user avatar
            getSelfUserPicture(plugin->name());
        }
    }
}

void SocialProxy::onSelfName(QString name)
{
    ISocialRequestManager *manager = dynamic_cast<ISocialRequestManager *>(sender());
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin * plugin, m_plugins) {
        bool isEnabled = settings.value(plugin->name()).toBool();
        if (isEnabled && plugin->authorized() && manager && (plugin->requestManager() == manager)) {
            plugin->setSelfName(name);
        }
    }
}

void SocialProxy::gotUserPictureUrl(QString userId, QString userPictureUrl)
{
    Q_UNUSED(userId)

    ISocialRequestManager *manager = dynamic_cast<ISocialRequestManager *>(sender());
    QSettings settings("ROSA", "Timeframe");

    foreach(ISocialPlugin * plugin, m_plugins) {
        bool isEnabled = settings.value(plugin->name()).toBool();
        if (isEnabled && plugin->authorized() && manager &&
                (plugin->requestManager() == manager) && userId == plugin->selfId()) {
            plugin->setSelfPictureUrl(userPictureUrl);
        }
    }
}

void SocialProxy::newComments(QString postId, QList<CommentItem *> items)
{
    m_socialModel->addComments(postId, items);

    // get all avatars of users
    foreach(CommentItem * item, items) {
        getUserPicture(item->data(CommentItem::FromId).toString(), postId, item->data(CommentItem::Type).toString());
    }
}

void SocialProxy::onSelfLiked(QString postId)
{
    m_socialModel->setSelfLiked(postId);
}

void SocialProxy::setSocialModel(SocialContentModel *model)
{
    m_socialModel = model;
}

ISocialPlugin *SocialProxy::pluginFromName(const QString &pluginName)
{
    ISocialPlugin *plugin = 0;
    foreach(ISocialPlugin * p, m_plugins) {
        if (p->name() == pluginName) {
            plugin = p;
            break;
        }
    }

    return plugin;
}
