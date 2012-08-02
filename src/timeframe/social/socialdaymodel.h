#ifndef SOCIALDAYMODEL_H
#define SOCIALDAYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
#include <QObject>

#include "../listmodel.h"

class SocialDayItem;
class SocialItem;


class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel( QObject * parent = 0 );

public slots:

    QObject* itemsModel(QDate date) const;
//    QString url( int row );

};


class SocialDayModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialDayModel( QHash<int, QByteArray> roles, QObject *parent = 0 );
     ~SocialDayModel();

    QVariant data( const QModelIndex &index, int role ) const;

    void appendRows(const QList<SocialDayItem*> &items);

public slots:
    void newSocialItems( QList<SocialItem*> list );
    QObject* itemsModel( QDate date ) const;


//    int getIndexByDate( int year, int month, bool direction );
//    QDate getDateOfIndex( int listIndex);
//    void setActivityType( const QString& type );

private slots:
    void handleItemChange();

private:
    QList<SocialDayItem *> m_items;

    QSet<QString> m_idSet;
};

#endif // SOCIALDAYMODEL_H
