#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <socialitem.h>
#include <QtCore/QMap>

class FeedItem : public SocialItem
{
public:
    FeedItem(const QVariantMap &map);
    virtual QString pluginName() const;
    virtual Type type() const;
    virtual QString id() const;
    virtual QVariant data( int role ) const;
    virtual QDate date() const;

private:
    void fillFromMap(const QVariantMap &map);

    QString m_id;
    QMap<int, QVariant> m_data;
    QList<CommentItem *> m_comments;
};

class FacebookCommentItem : public CommentItem
{
public:
    FacebookCommentItem(const QVariantList &map);
    virtual QString id() const;
    virtual QVariant data(int role) const;

private:
    void fillFromList( const QVariantList &comments );

    QMap<int, QVariant> m_data;
};

#endif // FEEDITEM_H
