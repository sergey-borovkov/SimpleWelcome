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

void SocialContentItem::addSocialItem(SocialItem* item)
{
    if (!m_types.contains(item->pluginName())) {
        m_types += (item->pluginName() + ";");
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

void SocialContentItem::setSocialFilter(const QRegExp& filter)
{
    m_model->setFilterRegExp(filter);
    emit dataChanged();
}
