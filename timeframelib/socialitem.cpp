#include "socialitem.h"

#include <commentitem.h>
#include <listmodel.h>

SocialItem::SocialItem(const QString &selfId)
    : m_selfId(selfId)
{
    m_commentsModel = new ListModel(CommentItem::roleNames());

    QVariant var;
    var.setValue(m_commentsModel);
    setData(Comments, var);
}

SocialItem::Type SocialItem::type() const
{
    return Post;
}

QString SocialItem::id() const
{
    return m_id;
}

void SocialItem::setId(const QString &id)
{
    m_id = id;
}

QVariant SocialItem::data(int role) const
{
    if (role == Id)
        return m_id;
    else
        return m_data.value(role);
}

bool SocialItem::setData(int role, const QVariant &value)
{
    m_data[role] = value;
    return true;
}

ListModel *SocialItem::commentsModel()
{
    return m_commentsModel;
}

QDateTime SocialItem::datetime() const
{
    return data(DateTime).toDateTime();
}
