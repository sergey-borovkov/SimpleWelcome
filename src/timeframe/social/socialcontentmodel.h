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

#ifndef SOCIALCONTENTMODEL_H
#define SOCIALCONTENTMODEL_H

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QDate>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>

class SocialContentItem;
class SocialItem;
class CommentItem;
class QDate;

class SocialContentModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialContentModel(QHash<int, QByteArray> roles, QObject *parent = 0);

    void appendRows(const QList<SocialContentItem *> &items);
    void insertRow(int row, SocialContentItem *item);

    Q_INVOKABLE QObject *itemsModel(QDate date) const;
    void resetModel();

public slots:
    void addComments(QString id, QList<CommentItem *> list);
    void setSelfLiked(QString id);
    void addCommentToItem(CommentItem *, QString);
    void likeItem(QString eventId);
    void newSocialItems(QList<SocialItem *> list);
    void removeItems(const QString &type);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &eventId);
    void updateUserName(const QString &userId, const QString &userName, const QString &eventId);
    void updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId);
    void updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId);
    void updateUserInfo(const QString &name, const QString &url, const QString &eventId);
private slots:
    void handleItemChange();

private:
    SocialContentItem *findItemByDate(const QDate &date) const;

    QHash<QString, QDate> m_idHash;
    QSet<QString> m_idSet;
};

class SocialContentFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialContentFilterModel(QObject *parent = 0);
    Q_INVOKABLE void setFilter(const QString &filter);
    Q_INVOKABLE void resetModel();
    Q_INVOKABLE QObject *itemsModel(QDate date) const;
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
    Q_INVOKABLE int getIndexByDate(int year, int month,  bool direction) const;
    Q_INVOKABLE int getIndexByDate(QDate date);
    Q_INVOKABLE int count();

};

#endif // SOCIALCONTENTMODEL_H
