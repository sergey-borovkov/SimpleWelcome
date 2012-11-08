#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <socialitem.h>
#include <listmodel.h>
#include <commentitem.h>

#include <QtCore/QMap>

class FeedItem : public SocialItem
{
public:
    FeedItem(const QVariantMap &map, QString selfId);
    virtual QString pluginName() const;
    virtual Type type() const;
    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);
    virtual QDateTime datetime() const;

private:
    void fillFromMap(QVariantMap map);
    QString convertSecsToStr(int secs);

    QString m_id;
    QMap<int, QVariant> m_data;
    QList<CommentItem *> m_comments;
    ListModel *m_commentsModel;
    QString m_selfId;
};

void fillCommentFromMap(CommentItem *item, const QVariantMap &map);

#endif // FEEDITEM_H
