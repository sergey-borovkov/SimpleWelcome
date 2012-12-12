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

#ifndef SOCIALDAYITEM_H
#define SOCIALDAYITEM_H

#include <QtCore/QDate>
#include <QtGui//QSortFilterProxyModel>

#include <listitem.h>

class CommentItem;
class SocialDayModel;
class SocialItem;
class SocialDayFilterModel;

class SocialContentItem : public QObject, public ListItem
{
    Q_OBJECT

public:
    enum Role {
        DateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        ItemsCountRole,
        ItemsTypes
    };

    explicit SocialContentItem(const QDate &date, QObject *parent = 0);
    ~SocialContentItem();

    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    QDate date();
    int   count();

    SocialDayFilterModel *model();

    QString types() const;
    void setSocialFilter(const QRegExp &);

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(DateRole,         "date");
        roles.insert(ItemsRole,        "items");
        roles.insert(CountRole,        "count");
        roles.insert(ItemsCountRole,   "size");
        roles.insert(ItemsTypes,       "type");

        return roles;
    }

signals:
    void dataChanged();

public slots:
    void setDate(const QDate &);
    void addSocialItem(SocialItem *item);
    void likeItem(QString eventId);
    void addCommentToItem(CommentItem *, QString);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id);
    void updateUserName(const QString &userId, const QString &userName, const QString &id);
    void updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId);
    void updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId);
    void updateUserInfo(const QString &name, const QString &url, const QString &eventId);
    void addComments(QString id, QList<CommentItem*> list);
    void setSelfLiked(QString id);

private:
    friend class SocialContentModel;

    QString m_types;
    QDate m_date;
    int m_count;
    SocialDayModel *m_itemModel;
    SocialDayFilterModel *m_model;
};

#endif // SOCIALDAYITEM_H
