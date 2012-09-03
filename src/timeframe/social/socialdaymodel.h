#ifndef SOCIALDAYMODEL_H
#define SOCIALDAYMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>
#include <QDate>
#include <QObject>

#include "../listmodel.h"

class SocialDayItem;
class SocialItem;


class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel(QObject * parent = 0);

public slots:
    void setFilter(const QString &filter);
    int getIndexByDate(int year, int month,  bool direction);
    QObject* itemsModel(QDate date) const;
    QDate getDateOfIndex(int listIndex);
//    QString url( int row );

};


class SocialDayModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    ~SocialDayModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void appendRows(const QList<SocialDayItem*> &items);
    void insertRow(int row, SocialDayItem* item);
    QModelIndex indexFromItem(const SocialDayItem *item) const;

public slots:
    void newSocialItems(QList<SocialItem*> list);
    QObject* itemsModel(QDate date) const;


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
