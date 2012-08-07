#ifndef LOCALDAYMODEL_H
#define LOCALDAYMODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QObject>
#include "localdayitem.h"
#include "itemmodel.h"

class ItemModel;
class Activity;
class ActivityProxy;

class TimeFrameDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeFrameDayFilterModel(QObject * parent = 0);
public slots:
    void setFilter(const QString &filter);
    QObject* itemsModel(QDate date) const;
    int getIndexByDate(int year, int month, bool direction);
    QDate getDateOfIndex(int listIndex);
};

class LocalDayModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        TypesRole,
        ItemsCountRole
    };
    explicit LocalDayModel(QObject *parent = 0);
     ~LocalDayModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void appendRow(LocalDayItem* item);
    void appendRows(const QList<LocalDayItem*> &items);
    void insertRow(int row, LocalDayItem *item);
    bool removeRow(int row, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    void setLister(ActivityProxy* lister);
    QModelIndex indexFromItem(const LocalDayItem *item) const;
    LocalDayItem * find(const QDate &date) const;
    void clear();

public slots:
    void newActivities(QList <Activity*> list);
    QObject* itemsModel(QDate date) const;
    int getIndexByDate(int year, int month, bool direction);
    QDate getDateOfIndex(int listIndex);
    void imageReady(QString url);    

protected:
    bool removeNullItem(int, int);

private slots:
    void newMonth(int, int);
    void handleItemChange();

private:
    QList <LocalDayItem *> m_items;
    QHash<int, QByteArray> hash;
    QHash<QString, QDate> m_urlHash;
    ActivityProxy* m_lister;
};

#endif // GALLERYMODEL_H
