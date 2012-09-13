#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <socialitem.h>
#include <QtCore/QMap>

#include "../timeframelib/listmodel.h"

class FeedItem : public SocialItem
{
public:
    FeedItem(const QVariantMap &map, QString selfId);
    virtual QString pluginName() const;
    virtual Type type() const;
    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, int role);
    virtual QDate date() const;

private:
    void fillFromMap(const QVariantMap &map);

    QString m_id;
    QMap<int, QVariant> m_data;
    QList<CommentItem *> m_comments;
    ListModel* m_commentsModel;
    QString m_selfId;
};

class FacebookCommentItem : public CommentItem
{
public:
    FacebookCommentItem(const QVariantMap &map);
    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, int role);

private:
    void fillFromMap(const QVariantMap &map);

    QMap<int, QVariant> m_data;
};


#endif // FEEDITEM_H
