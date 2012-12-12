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

#ifndef PLUGINREPLY_H
#define PLUGINREPLY_H

#include <QtCore/QObject>

class Request;

/**
 * @brief The PluginReply class encapsulates replies from social plugins
 *        and allows to read those replies from QML. This class is necessary because
 *        Request is interface and can not be used from QML (and it does not have signals
 *        plugins are supposed to emit for the same reasons).
 */
class PluginReply : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool finished READ isFinished NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString)
public:
    /**
     * @brief PluginReply
     * @param request Pointer to Request
     * @param parent
     */
    PluginReply(Request *request, const QString &sourceId, const QString &pluginName, QObject *parent = 0);

    /**
     * @brief isComplete Check if request already completed
     * @return bool
     */
    Q_INVOKABLE bool isFinished() const;

    /**
     * @brief Return new item's id if it was created
     * @return id  Id string or empty string if no new objects were created
     */
    Q_INVOKABLE QString id() const;

    /**
     * @brief errorString
     * @return last error string
     */
    Q_INVOKABLE QString errorString() const;

    QString sourceId();

    Q_INVOKABLE QString userPictureUrl() const;
    Q_INVOKABLE QString userId() const;
    Q_INVOKABLE QString userName() const;

    Q_INVOKABLE QString audioId() const;
    Q_INVOKABLE QString audioOwnerId() const;
    Q_INVOKABLE QString audioUrl() const;

    Q_INVOKABLE QString videoId() const;
    Q_INVOKABLE QString videoOwnerId() const;
    Q_INVOKABLE QString videoUrl() const;
    Q_INVOKABLE QString videoImage() const;

    Q_INVOKABLE QString pluginName() const;

signals:
    void success(PluginReply *);
    void failure(PluginReply *);
    void finished();

private slots:
    void requestSuccess();
    void newItemId(QString id);
    void gotUserPictureUrl(QString id, QString url);
    void gotAudioUrl(QString aid, QString ownerId, QString url);
    void gotVideoUrl(QString vid, QString ownerId, QString url, QString image);
    void gotUserName(QString id, QString name);
    void gotUserInfo(QString id, QString name, QString url);
    void error(QString error);

private:
    QString m_sourceId;
    QString m_id;
    QString m_errorString;

    QString m_userId;
    QString m_userPictureUrl;
    QString m_userName;

    QString m_pluginName;

    QString m_audioId;
    QString m_audioOwnerId;
    QString m_audioUrl;

    QString m_videoId;
    QString m_videoOwnerId;
    QString m_videoUrl;
    QString m_videoImage;

    bool m_finished;
};

#endif // PLUGINREQUESTREPLY_H
