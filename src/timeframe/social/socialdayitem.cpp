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

#include "socialdayitem.h"
#include "socialdaymodel.h"
#include "socialitem.h"


SocialContentItem::SocialContentItem(const QDate &date, QObject *parent)
    : QObject(parent)
    , m_date(date)
{
    m_itemModel = new SocialDayModel(SocialItem::roleNames(), this);
    m_model = new SocialDayFilterModel(this);
    m_model->setSourceModel(m_itemModel);
    m_model->setDynamicSortFilter(true);
}

SocialContentItem::~SocialContentItem()
{
}

QVariant SocialContentItem::data(int role) const
{
    if (role == DateRole) {
        return QVariant(m_date);
    } else if (role == ItemsCountRole) {
        return QVariant(m_model->rowCount(QModelIndex()));
    } else if (role == ItemsTypes) {
        return QVariant(types());
    }

    return QVariant();
}

bool SocialContentItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    return false;
}

QString SocialContentItem::id() const
{
    return QString();
}

void SocialContentItem::setDate(const QDate &d)
{
    m_date = d;
    emit dataChanged();
}

void SocialContentItem::addSocialItem(SocialItem *item)
{
    if (!m_types.contains(item->data(SocialItem::PluginName).toString())) {
        m_types += (item->data(SocialItem::PluginName).toString() + ";");
    }
    m_itemModel->addSocialItem(item);
    emit dataChanged();
}

void SocialContentItem::likeItem(QString eventId)
{
    m_itemModel->like(eventId);
    m_model->update();
}

void SocialContentItem::addCommentToItem(CommentItem *item, QString id)
{
    m_itemModel->addComment(item, id);
    m_model->update();
}


void SocialContentItem::updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id)
{
    m_itemModel->updateUserImage(userId, userImageUrl, id);
    m_model->update();
}

void SocialContentItem::updateUserName(const QString &userId, const QString &userName, const QString &id)
{
    m_itemModel->updateUserName(userId, userName, id);
    m_model->update();
}

void SocialContentItem::updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId)
{
    m_itemModel->updateAudioUrl(audioId, audioOwnerId, audioUrl, eventId);
    m_model->update();
}

void SocialContentItem::updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId)
{
    m_itemModel->updateVideoUrl(videoId, videoOwnerId, videoUrl, videoImage, eventId);
    m_model->update();
}

void SocialContentItem::updateUserInfo(const QString &name, const QString &url, const QString &eventId)
{
    m_itemModel->updateUserInfo(name, url, eventId);
    m_model->update();
}

void SocialContentItem::addComments(QString id, QList<CommentItem *> list)
{
    m_itemModel->addComments(id, list);
}

void SocialContentItem::setSelfLiked(QString id)
{
    m_itemModel->setSelfLiked(id);
    m_model->update();
}

QDate SocialContentItem::date()
{
    return m_date;
}

int SocialContentItem::count()
{
    return m_model->rowCount(QModelIndex());
}

SocialDayFilterModel *SocialContentItem::model()
{
    return m_model;
}

QString SocialContentItem::types() const
{
    return m_types;
}

void SocialContentItem::setSocialFilter(const QRegExp &filter)
{
    m_model->setFilterRegExp(filter);
    emit dataChanged();
}
