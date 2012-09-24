#include "socialitemmodel.h"

#include <QDate>
#include <QDebug>

#include "socialitem.h"
#include "socialproxy.h"

#include <commentitem.h>

SocialItemModel::SocialItemModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

SocialItemModel::~SocialItemModel()
{
}

/*
 * add social item to model
 */
void SocialItemModel::addSocialItem(SocialItem *item)
{
    if(!item) {
        return;
    }

    /* Check duplicates */
    QString strId = QString("%1-%2").arg(item->pluginName()).arg(item->id());
    if(m_idSet.contains(strId)) {
        return;
    }
    m_idSet.insert(strId);

    appendRow(item);
}

void SocialItemModel::like(QString id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i,0),SocialItem::Id).toString() == id) {
            int likesCount = data(index(i,0),SocialItem::Likes).toInt();
            int liked = data(index(i,0),SocialItem::Like).toInt();
            switch(liked){
            case SocialItem::NotLiked :
                 setData(index(i,0), SocialItem::Liked, SocialItem::Like);
                 setData(index(i,0),++likesCount, SocialItem::Likes);
                 break;
            case SocialItem::Liked :
                setData(index(i,0), SocialItem::NotLiked, SocialItem::Like);
                setData(index(i,0),--likesCount, SocialItem::Likes);
                break;
            case SocialItem::LikeNotAvailable :
                break;
            }
            break;
        }
    }
}

void SocialItemModel::addComment(CommentItem *item, QString id)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i,0),SocialItem::Id).toString() == id) {
            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel * commentsModel = qvariant_cast<ListModel* >(v);
            commentsModel->appendRow(item);
            int commentCount = data(index(i, 0), SocialItem::CommentCount).toInt();
            bool result = setData(index(i,0), ++commentCount, SocialItem::CommentCount);
            if (!result)
                qDebug() << " error on set comments count";
            emit dataChanged(index(i, 0),index(i, 0));
        }
    }
}

void SocialItemModel::addComments(QString id, QList<CommentItem *> list)
{
    for (int i = 0; i < rowCount(); i++) {
        if (data(index(i,0),SocialItem::Id).toString() == id) {
            QVariant v = data(index(i, 0), SocialItem::Comments);
            ListModel * commentsModel = qvariant_cast<ListModel* >(v);            
            QString firstPostId = commentsModel->data(index(0,0),CommentItem::Id).toString();
            for (int j = 0; j < list.size(); j++) {
                CommentItem* item = list.at(j);
                if (item->id() == firstPostId) {
                    break;
                }
                commentsModel->insertRow(j, item);
            }
            emit dataChanged(index(i, 0),index(i, 0));
        }
    }

}
