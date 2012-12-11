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

#ifndef SOCIALITEMMODEL_H
#define SOCIALITEMMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>

#include <listmodel.h>
#include <QDate>

class SocialItem;
class CommentItem;

class SocialDayModel : public ListModel
{
    Q_OBJECT

public:
    explicit SocialDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

public slots:
    void addComment(CommentItem *item, QString id);
    void addComments(QString id, QList<CommentItem *> list);
    void setSelfLiked(QString id);
    void addSocialItem(SocialItem *item);
    void like(QString id);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id);
    void updateUserName(const QString &userId, const QString &userName, const QString &id);
    void updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId);
    void updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId);
    void updateUserInfo(const QString &name, const QString &url, const QString &eventId);

private:
    QSet<QString> m_idSet;
};

class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel(QObject *parent = 0);

signals:
    /**
     * @brief updateData - signal is emitted for not-view-based delegates to update role values
     */
    void updateData();

public slots:
    QString id(int row) const;
    QString imageUrl(int row) const;
    QString audio(int row) const;
    QString audioUrl(int row) const;
    QString video(int row) const;
    QString videoUrl(int row) const;
    QString fromName(int row) const;
    QString fromImageUrl(int row) const;
    QString text(int row) const;
    QDateTime datetime(int row) const;
    int likesCount(int row) const;
    int like(int row);
    int commentsCount(int row) const;
    QObject *comments(int row) const;
    QString pluginName(int row) const;
    void update();
};

Q_DECLARE_METATYPE(SocialDayModel *)

#endif // SOCIALITEMMODEL_H
