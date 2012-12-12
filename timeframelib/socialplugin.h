/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#ifndef SOCIALPLUGIN_H
#define SOCIALPLUGIN_H

#include <QtCore/QList>
#include <QtCore/QtPlugin>
#include <QtGui/QPixmap>

class SocialItem;
class QDate;

/**
 * @brief The Request class encapsulates all write requests to social plugins .
 *        All subclasses of this class in social plugins should emit success()
 *        and error(QString errorString) signals when appropriate.
 */
class Request
{
public:
    virtual ~Request() {}
    /**
     * @brief start Start request. This method must be not blocking.
     */
    virtual void start() = 0;
};

class ISocialRequestManager
{
public:
    virtual ~ISocialRequestManager() {}
    virtual Request *queryWall() = 0;
    virtual Request *queryUserId() = 0;
    virtual Request *queryImage(const QString &id) = 0;
    virtual Request *queryAudio(const QString &aid, const QString &ownerId) = 0;
    virtual Request *queryVideo(const QString &vid, const QString &ownerId) = 0;
    virtual Request *queryUserInfo(const QString &fromId) = 0;
    virtual Request *postComment(const QByteArray &message, const QString &parentId) = 0;
    virtual Request *postToWall(const QByteArray &message) = 0;

    /**
     * @brief Query comments to post. You need to connect to SocialRequestManager
     *        newComments(QString parent, QList<CommentItem *>) signal to get result
     * @param id of parent post
     * @return pointer to request
     */
    virtual Request *queryComments(const QString &postId) = 0;

    virtual Request *queryLikes(const QString &postId) = 0;
    virtual Request *like(const QString &id) = 0;
    virtual Request *unlike(const QString &id) = 0;

    virtual Request *logout() = 0;
};

class ISocialPlugin
{
public:
    enum AuthorizationStatus {
        Success,
        Expired,
        Failure
    };

    virtual ~ISocialPlugin() {}
    virtual QString name() const = 0;
    virtual QPixmap icon() const = 0;
    virtual QPixmap smallIcon() const = 0;
    virtual QWidget *authenticationWidget() = 0;
    virtual ISocialRequestManager *requestManager() = 0;
    virtual bool authorized() const = 0;
    virtual QString displayName() const = 0;

    virtual QString selfId() const = 0;
    virtual void setSelfId(const QString &id) = 0;

    virtual QString selfName() const = 0;
    virtual void setSelfName(const QString &name) = 0;

    virtual QString selfPictureUrl() const = 0;
    virtual void setSelfPictureUrl(const QString &url) = 0;
};

Q_DECLARE_INTERFACE(ISocialPlugin, "Timeframe_Library.SocialModule/1.0")

#endif // SOCIALPLUGIN_H
