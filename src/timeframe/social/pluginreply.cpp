/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
 *  Julia Mineeva <julia.mineeca@osinit.ru>
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

#include "pluginreply.h"
#include <socialplugin.h>

PluginReply::PluginReply(Request *request, const QString &sourceId, const QString &pluginName, QObject *parent)
    : QObject(parent)
    , m_sourceId(sourceId)
    , m_pluginName(pluginName)
    , m_finished(false)
{
    QObject *r = dynamic_cast<QObject *>(request);
    if (r != 0) {
        connect(r, SIGNAL(success()), SLOT(requestSuccess()));
        connect(r, SIGNAL(newItemId(QString)), SLOT(newItemId(QString)));
        connect(r, SIGNAL(gotUserPictureUrl(QString, QString)), SLOT(gotUserPictureUrl(QString, QString)));
        connect(r, SIGNAL(error(QString)), SLOT(error(QString)));
    }
}

bool PluginReply::isFinished() const
{
    return m_finished;
}

QString PluginReply::id() const
{
    return m_id;
}

QString PluginReply::errorString() const
{
    return m_errorString;
}

QString PluginReply::sourceId()
{
    return m_sourceId;
}

QString PluginReply::userPictureUrl() const
{
    return m_userPictureUrl;
}

QString PluginReply::userId() const
{
    return m_userId;
}

QString PluginReply::userName() const
{
    return m_userName;
}

QString PluginReply::audioId() const
{
    return m_audioId;
}

QString PluginReply::audioOwnerId() const
{
    return m_audioOwnerId;
}

QString PluginReply::audioUrl() const
{
    return m_audioUrl;
}

QString PluginReply::videoId() const
{
    return m_videoId;
}

QString PluginReply::videoOwnerId() const
{
    return m_videoOwnerId;
}

QString PluginReply::videoUrl() const
{
    return m_videoUrl;
}

QString PluginReply::videoImage() const
{
    return m_videoImage;
}

QString PluginReply::pluginName() const
{
    return m_pluginName;
}

void PluginReply::newItemId(QString id)
{
    m_id = id;
}

void PluginReply::gotUserPictureUrl(QString id, QString url)
{
    m_userId = id;
    m_userPictureUrl = url;
}

void PluginReply::gotAudioUrl(QString aid, QString ownerId, QString url)
{
    m_audioId = aid;
    m_audioOwnerId = ownerId;
    m_audioUrl = url;
}

void PluginReply::gotVideoUrl(QString vid, QString ownerId, QString url, QString image)
{
    m_videoId = vid;
    m_videoOwnerId = ownerId;
    m_videoUrl = url;
    m_videoImage = image;
}

void PluginReply::gotUserName(QString id, QString name)
{
    m_userId = id;
    m_userName = name;
}

void PluginReply::gotUserInfo(QString id, QString name, QString url)
{
    m_userId = id;
    m_userName = name;
    m_userPictureUrl = url;
}

void PluginReply::error(QString error)
{
    m_errorString = error;
    m_finished = true;
    emit failure(this);
    emit finished();
}
void PluginReply::requestSuccess()
{
    m_finished = true;
    emit success(this);
    emit finished();
}
