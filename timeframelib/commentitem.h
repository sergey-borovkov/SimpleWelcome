#ifndef COMMENTITEM_H
#define COMMENTITEM_H

#include <QtCore/QMetaType>
#include <QtCore/QHash>
#include <QtCore/QMap>
#include <QtCore/QByteArray>
#include <QtCore/QVariant>

#include "listitem.h"

class TIMEFRAMELIB_EXPORT CommentItem : public ListItem
{
public:
    enum Role {
        From = Qt::UserRole + 1,
        FromId,
        Message,
        CreatedTime,
        LikeCount,
        Id
    };
    ~CommentItem();

    virtual QString id() const;

    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(From, "from" );
        roles.insert(FromId, "fromId" );
        roles.insert(Message, "messageText" );
        roles.insert(CreatedTime, "createdTime" );
        roles.insert(LikeCount, "likeCount" );
        roles.insert(Id, "id" );
        return roles;
    }

private:
    QMap<int, QVariant> m_data;
};

Q_DECLARE_METATYPE( CommentItem * )
Q_DECLARE_METATYPE(QList<CommentItem *>)

#endif