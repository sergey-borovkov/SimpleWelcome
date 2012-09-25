#include "commentitem.h"

CommentItem::~CommentItem()
{
}

QString CommentItem::id() const
{
    return data(Id).toString();
}

QVariant CommentItem::data(int role) const
{
    if(role == CommentItem::FromPictureUrl) {
        if (m_data.value(role).toString().isEmpty())
            return QVariant("images/user.png");
    }
    return m_data.value(role);
}

bool CommentItem::setData(int role, const QVariant &value)
{
    m_data.insert(role, value);
    return true;
}
