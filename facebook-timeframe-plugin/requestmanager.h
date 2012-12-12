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

#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <socialplugin.h>

#include <QtCore/QUrl>
#include <QtCore/QMap>
#include <QtCore/QVariant>

class OAuth2Authorizer;
class SocialItem;
class CommentItem;
class QNetworkAccessManager;

const QString openGraphUrl = QLatin1String("https://graph.facebook.com/");

class RequestManager : public QObject, public ISocialRequestManager
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    virtual Request *queryWall();
    virtual Request *queryUserId();
    virtual Request *queryImage(const QString &id);
    virtual Request *queryAudio(const QString &aid, const QString &ownerId);
    virtual Request *queryVideo(const QString &vid, const QString &ownerId);
    virtual Request *queryUserInfo(const QString &fromId);
    virtual Request *queryComments(const QString &postId);
    virtual Request *queryLikes(const QString &postId);
    virtual Request *postComment(const QByteArray &message, const QString &parentId);
    virtual Request *postToWall(const QByteArray &message);
    virtual Request *like(const QString &id);
    virtual Request *unlike(const QString &id);

    void setAuthorizer(OAuth2Authorizer *authorizer);
    virtual Request *logout();

    virtual QString pluginName() const;

private slots:
    void feedReply(QByteArray feedReply);
    void idReply(QByteArray reply);
    void commentReply(QByteArray reply);
    void imageReply(QByteArray reply);

signals:
    void authorizationComplete();
    void searchComplete();
    void newSocialItems(QList<SocialItem *> items);
    void selfId(QString id);
    void selfName(QString name);
    void selfLiked(QString postId);
    void newComments(QString postId, QList<CommentItem *> items);
    void gotUserImage(QString id, QString url);
    void gotUserName(QString postId, QString name);
    void gotUserInfo(QString id, QString name, QString url);

private:
    QUrl constructUrl(const QString &id, const QString &type) const;
    void fillFromMap(SocialItem *item, const QVariantMap &map);

    OAuth2Authorizer *m_authorizer;

    QString m_selfId;
    QString m_selfName;

    // we return comments only when they are fully downloaded
    // so we need to keed partial results between calls somewhere
    QMap<QString, QList<CommentItem *> >m_comments;
};

void fillCommentFromMap(CommentItem *item, const QVariantMap &map);

#endif // REQUESTMANAGER_H
