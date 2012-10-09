#ifndef LOCALCONTENTMODEL_H
#define LOCALCONTENTMODEL_H

#include <QtCore/QObject>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>

class ItemModel;
class Activity;
class ActivityProxy;
class LocalDayItem;

/**
 * @brief The LocalContentModel class stores local content
 *        by date, where every item keeps all content for
 *        certain day
 */
class LocalContentModel : public ListModel
{
    Q_OBJECT
public:
    explicit LocalContentModel(QHash<int, QByteArray> roles, QObject *parent = 0);
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

/**
 * @brief The LocalContentFilterModel class allows filtering
 *        of LocalContentModel by content type - video/documents/images
 *        or everthing togethere
 */
class LocalContentFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalContentFilterModel(QObject * parent = 0);
    Q_INVOKABLE void setFilter(const QString &filter);
    Q_INVOKABLE QObject* itemsModel(QDate date) const;
    Q_INVOKABLE int getIndexByDate(int year, int month, bool direction);
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
};

#endif // LOCALCONTENTMODEL_H
