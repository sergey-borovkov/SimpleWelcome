#include "socialitem.h"
#include "socialdaymodel.h"
#include "socialproxy.h"

#include <commentitem.h>

#include <QtCore/QDateTime>
#include <QtCore/QDebug>

SocialDayModel::SocialDayModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

bool SocialDayModel::removeRow(int row, const QModelIndex &parent)
{
    SocialItem *item = static_cast<SocialItem *>(itemAt(row));
    QString id = QString("%1-%2").arg(item->data(SocialItem::PluginName).toString()).arg(item->id());
    m_idSet.remove(id);
    return ListModel::removeRow(row, parent);
}

/*
 * add social item to model
 */
void SocialDayModel::addSocialItem(SocialItem *item)
{
    if (!item) {
        return;
    }
    /* Check duplicates */
    QString strId = QString("%1-%2").arg(item->data(SocialItem::PluginName).toString()).arg(item->id());
    if (m_idSet.contains(strId)) {
        return;
    }
    m_idSet.insert(strId);

    // add sorted items (new item to the end of list)
    if (rowCount() > 0 && item->datetime() <= data(index(0, 0), SocialItem::DateTime).toDateTime())
        prependRow(item);
    else
        appendRow(item);
}

void SocialDayModel::like(QString id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {
            int likesCount = data(index(i, 0), SocialItem::Likes).toInt();
            int liked = data(index(i, 0), SocialItem::Like).toInt();
            switch (liked) {
            case SocialItem::NotLiked :
                setData(index(i, 0), SocialItem::Liked, SocialItem::Like);
                setData(index(i, 0), ++likesCount, SocialItem::Likes);
                break;
            case SocialItem::Liked :
                setData(index(i, 0), SocialItem::NotLiked, SocialItem::Like);
                setData(index(i, 0), --likesCount, SocialItem::Likes);
                break;
            case SocialItem::LikeNotAvailable :
                break;
            }
            break;
        }
    }
}

void SocialDayModel::addComment(CommentItem *item, QString id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {
            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel *commentsModel = qvariant_cast<ListModel * >(v);
            if (commentsModel)
                commentsModel->appendRow(item);
            int commentCount = data(index(i, 0), SocialItem::CommentCount).toInt();
            bool result = setData(index(i, 0), ++commentCount, SocialItem::CommentCount);
            if (!result)
                qWarning("Error on set comments count");
            emit dataChanged(index(i, 0), index(i, 0));
        }
    }
}

void SocialDayModel::updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {

            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel *commentsModel = qvariant_cast<ListModel * >(v);
            if (commentsModel)
                for (int j = 0; j < commentsModel->rowCount(); j++) {
                    if (commentsModel->data(commentsModel->index(j, 0), CommentItem::FromId).toString() == userId) {
                        commentsModel->setData(commentsModel->index(j, 0), QVariant(userImageUrl), CommentItem::FromPictureUrl);
                    }
                }
        }
    }
}

void SocialDayModel::updateUserName(const QString &userId, const QString &userName, const QString &id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {

            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel *commentsModel = qvariant_cast<ListModel * >(v);
            if (commentsModel)
                for (int j = 0; j < commentsModel->rowCount(); j++) {
                    if (commentsModel->data(commentsModel->index(j, 0), CommentItem::FromId).toString() == userId) {
                        commentsModel->setData(commentsModel->index(j, 0), QVariant(userName), CommentItem::From);
                    }
                }
        }
    }
}

void SocialDayModel::updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == eventId) {
            if (!audioId.isEmpty())
                setData(index(i, 0), QVariant(audioId), SocialItem::AudioId);
            if (!audioOwnerId.isEmpty())
                setData(index(i, 0), QVariant(audioOwnerId), SocialItem::AudioOwnerId);
            if (!audioUrl.isEmpty())
                setData(index(i, 0), QVariant(audioUrl), SocialItem::AudioUrl);
        }
    }
}

void SocialDayModel::updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == eventId) {
            if (!videoId.isEmpty())
                setData(index(i, 0), QVariant(videoId), SocialItem::VideoId);
            if (!videoOwnerId.isEmpty())
                setData(index(i, 0), QVariant(videoOwnerId), SocialItem::VideoOwnerId);
            if (!videoUrl.isEmpty())
                setData(index(i, 0), QVariant(videoUrl), SocialItem::VideoUrl);
            if (!videoImage.isEmpty())
                setData(index(i, 0), QVariant(videoImage), SocialItem::VideoImage);
        }
    }
}

void SocialDayModel::updateUserInfo(const QString &name, const QString &url, const QString &eventId)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == eventId) {
            if (!name.isEmpty())
                setData(index(i, 0), QVariant(name), SocialItem::FromName);
            if (!url.isEmpty())
                setData(index(i, 0), QVariant(url), SocialItem::FromImageUrl);
        }
    }
}

void SocialDayModel::addComments(QString id, QList<CommentItem *> list)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {
            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel *commentsModel = qvariant_cast<ListModel * >(v);
            if (commentsModel) {
                QString firstPostId = commentsModel->data(index(0, 0), CommentItem::Id).toString();
                for (int j = 0; j < list.size(); j++) {
                    CommentItem *item = list.at(j);
                    if (item->id() == firstPostId) {
                        break;
                    }
                    commentsModel->insertRow(j, item);
                }
                emit dataChanged(index(i, 0), index(i, 0));
            }
        }
    }
}

void SocialDayModel::setSelfLiked(QString id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i, 0), SocialItem::Id).toString() == id) {
            setData(index(i, 0), SocialItem::Liked, SocialItem::Like);
        }
    }
}

SocialDayFilterModel::SocialDayFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(SocialItem::PluginName);
}

QString SocialDayFilterModel::id(int row) const
{
    return data(index(row, 0), SocialItem::Id).toString();
}

QString SocialDayFilterModel::imageUrl(int row) const
{
    return data(index(row, 0), SocialItem::ImageUrl).toString();
}

QString SocialDayFilterModel::audio(int row) const
{
    return data(index(row, 0), SocialItem::Audio).toString();
}

QString SocialDayFilterModel::audioUrl(int row) const
{
    return data(index(row, 0), SocialItem::AudioUrl).toString();
}

QString SocialDayFilterModel::video(int row) const
{
    return data(index(row, 0), SocialItem::Video).toString();
}

QString SocialDayFilterModel::videoUrl(int row) const
{
    return data(index(row, 0), SocialItem::VideoUrl).toString();
}

QString SocialDayFilterModel::fromName(int row) const
{
    return data(index(row, 0), SocialItem::FromName).toString();
}

QString SocialDayFilterModel::fromImageUrl(int row) const
{
    return data(index(row, 0), SocialItem::FromImageUrl).toString();
}

QString SocialDayFilterModel::text(int row) const
{
    return data(index(row, 0), SocialItem::Text).toString();
}

QDateTime SocialDayFilterModel::datetime(int row) const
{
    return data(index(row, 0), SocialItem::DateTime).toDateTime();
}

int SocialDayFilterModel::likesCount(int row) const
{
    return data(index(row, 0), SocialItem::Likes).toInt();
}

int SocialDayFilterModel::like(int row)
{
    return data(index(row, 0), SocialItem::Like).toInt();
}

int SocialDayFilterModel::commentsCount(int row) const
{
    return data(index(row, 0), SocialItem::CommentCount).toInt();
}

QObject *SocialDayFilterModel::comments(int row) const
{
    QVariant v = data(index(row, 0), SocialItem::Comments);
    ListModel *commentsModel = qvariant_cast<ListModel * >(v);
    if (commentsModel)
        commentsModel->setParent(sourceModel());
    return commentsModel;
}

QString SocialDayFilterModel::pluginName(int row) const
{
    return data(index(row, 0), SocialItem::PluginName).toString();
}

void SocialDayFilterModel::update()
{
    emit updateData();
}
