#ifndef LOCALDAYMODEL_H
#define LOCALDAYMODEL_H

#include "itemmodel.h"
#include "localdayitem.h"

#include <QtCore/QObject>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>

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

class LocalDayModel : public ListModel
{
    Q_OBJECT
public:
    explicit LocalDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(LocalDayItem* item);
    void appendRows(const QList<LocalDayItem*> &items);
    void insertRow(int row, LocalDayItem *item);
    void setLister(ActivityProxy* lister);
    LocalDayItem * find(const QDate &date) const;

    QRegExp filter() const;
    void setFilter(QRegExp regexp);

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
    QHash<QString, QDate> m_urlHash;
    ActivityProxy* m_lister;
    QRegExp m_filter;
};

#endif // GALLERYMODEL_H
