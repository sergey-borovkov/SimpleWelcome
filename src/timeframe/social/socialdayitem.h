#ifndef SOCIALDAYITEM_H
#define SOCIALDAYITEM_H

#include <QtCore/QDate>
#include <QtGui//QSortFilterProxyModel>

#include <listitem.h>

class CommentItem;
class SocialDayModel;
class SocialItem;
class SocialDayFilterModel;

class SocialContentItem : public QObject, public ListItem
{
    Q_OBJECT

public:
    enum Role {
        DateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        ItemsCountRole,
        ItemsTypes
    };

    explicit SocialContentItem(const QDate &date, QObject *parent = 0);
    ~SocialContentItem();

    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    QDate date();
    int   count();

    SocialDayFilterModel *model();

    QString types() const;
    void setSocialFilter(const QRegExp&);

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(DateRole,         "date");
        roles.insert(ItemsRole,        "items");
        roles.insert(CountRole,        "count");
        roles.insert(ItemsCountRole,   "size");
        roles.insert(ItemsTypes,       "type");

        return roles;
    }

signals:
    void dataChanged();

public slots:
    void setDate(const QDate&);
    void addSocialItem(SocialItem* item);
    void likeItem(QString eventId);
    void addCommentToItem(CommentItem *, QString);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id);
    void updateUserName(const QString &userId, const QString &userName, const QString &id);
    void addComments(QString id, QList<CommentItem*> list);
    void setSelfLiked(QString id);

private:
    friend class SocialContentModel;

    QString m_types;
    QDate m_date;
    int m_count;
    SocialDayModel *m_itemModel;
    SocialDayFilterModel *m_model;
};

#endif // SOCIALDAYITEM_H
