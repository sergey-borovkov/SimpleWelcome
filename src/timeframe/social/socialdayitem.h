#ifndef SOCIALDAYITEM_H
#define SOCIALDAYITEM_H

#include <QStringList>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QObject>

#include <listitem.h>

class CommentItem;
class SocialItemModel;
class SocialItem;
class SocialItemFilterModel;

class SocialItemFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialItemFilterModel(QObject * parent = 0);

signals:
    /**
     * @brief updateData - signal is emitted for not-view-based delegates to update role values
     */
    void updateData();

public slots:

    QString id(int row);
    QString imageUrl(int row);
    QString text(int row);
    int likesCount(int row);
    int like(int row);
    int commentsCount(int row);
    QObject *comments(int row);
    QString pluginName(int row);
    void update();
};


class SocialDayItem : public QObject, public ListItem
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

    explicit SocialDayItem(const QDate &date, QObject *parent = 0);
    ~SocialDayItem();

    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    QDate date();
    int   count();

    SocialItemFilterModel *model();

    QString types() const;
    void setSocialFilter(const QRegExp&);

    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(DateRole,         "date");
        roles.insert(ItemsRole,        "items");
        roles.insert(CountRole,        "count");
        roles.insert(ItemsCountRole,   "size");
        roles.insert(ItemsTypes,      "type");

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
    void addComments(QString id, QList<CommentItem*> list);

private:
    QString m_types;
    QDate       m_date;
    int         m_count;
    SocialItemModel         *m_itemModel;
    SocialItemFilterModel   *m_model;

    friend class SocialDayModel;
};

#endif // SOCIALDAYITEM_H
