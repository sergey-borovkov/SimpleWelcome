#ifndef SOCIALDAYITEM_H
#define SOCIALDAYITEM_H

#include <QStringList>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QObject>

class SocialItemModel;
class SocialItem;
class SocialItemFilterModel;

#include "../listitem.h"

class SocialItemFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialItemFilterModel( QObject * parent = 0 );

signals:

public slots:
//    QString url(int row);

};


class SocialDayItem : public QObject, public ListItem
{
    Q_OBJECT

public:
    enum Role
    {
        DateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        ItemsCountRole,
        ItemsTypes
    };

    explicit SocialDayItem( const QDate &date, QObject *parent = 0 );
    ~SocialDayItem();

    virtual QString id() const;
    virtual QVariant data( int role ) const;

    QDate date();
    int   count();

    SocialItemFilterModel *model();

    QString types() const;
    void setSocialFilter( const QRegExp& );

    static const QHash<int, QByteArray> roleNames()
    {
        QHash<int, QByteArray> roles;
        roles.insert( DateRole,         "date" );
        roles.insert( ItemsRole,        "items" );
        roles.insert( CountRole,        "count" );
        roles.insert( ItemsCountRole,   "size" );
        roles.insert( ItemsTypes,      "type" );

        return roles;
    }

signals:
    void dataChanged();

public slots:
    void setDate( const QDate& );
    void addSocialItem( SocialItem* item );

private:
    QString m_types;
    QDate       m_date;
    int         m_count;
    SocialItemModel         *m_itemModel;
    SocialItemFilterModel   *m_model;
};

#endif // SOCIALDAYITEM_H
