#include "activitymodel.h"
#include "activitysource.h"
#include "activityset.h"

#include <QDate>
#include <QList>
#include <QDebug>
#include <QtAlgorithms>


uint qHash(const QDate &date)
{
    return qHash(date.toString());
}

ActivityModel::ActivityModel(QObject *parent) :
    QAbstractListModel(parent), currentDate(QDate::currentDate()), lastSearchedDate(QDate::currentDate()), days(0)
{
    QHash<int, QByteArray> roles = roleNames();
    roles.insert(CurrentDateRole, QByteArray("currentDate"));
    roles.insert(ActivitiesRole, QByteArray("activities"));
    setRoleNames(roles);
    qDebug() << roles;
}

ActivityModel::~ActivityModel()
{
}

int ActivityModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
    Q_UNUSED(parent)
    return days;
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row >= 0)
    {
        if ( role == CurrentDateRole )
        {
            return QDate::currentDate().addDays(-row);
        }
        else if ( role == ActivitiesRole )
        {
            //QDate requestedDate = QDate::currentDate().addDays(-row);

            qDebug( "row is %d", row);
         //   qDebug( "date is %s", requestedDate.toString().toLocal8Bit().data() );
             QDate key = map.keys()[ index.row() ];

             QList<Activity *> values = map.values(key);
             ActivitySet *set = new ActivitySet(values, 0);
             QVariant var;
             var.setValue(set);
             return var;
        }
    }

    return QVariant();
}

void ActivityModel::addSource(ActivitySource *source)
{
    this->source = source; // remember pointer to source to be able to use blocking API if necessary

    connect(source, SIGNAL(newActivities(QList<Activity*>)), SLOT(addActivities(QList<Activity*>)));
    connect(this, SIGNAL(newSearch(QDate)), source, SLOT(startSearch(QDate)));
    connect(source, SIGNAL(finishedListing()), SLOT(listingFinished()));

}

void ActivityModel::addActivities(QList<Activity *> list)
{
    for(int i = 0; i < list.size(); i++)
    {
        QList<QDate> keys = map.keys();

        QSet<QDate> set = QSet<QDate>::fromList(keys);
        keys = set.toList();

        qDebug() << "KEYS" << keys;
        QList<QDate>::const_iterator it = qLowerBound(keys, list[i]->getDate());

        qDebug() << "IT" << it.i;

        int row = it - keys.constBegin();
        qDebug() << "ROW" << row;

        if(!map.contains(list[i]->getDate()))
        {                
                beginInsertRows(QModelIndex(), row, row + 1);

                days++;
                map.insert(list[i]->getDate(), list[i]);

                endInsertRows();
        }
        else
        {
            Activity *val = map.value(list[i]->getDate());
            qDebug() << list[i]->getDate() << list[i]->getUrl();

            if((val && val->getUrl() != list[i]->getUrl()) || !val)
            {
                map.insert(list[i]->getDate(), list[i]);
                emit dataChanged(index(row), index(row));
            }
            else
            {
                qDebug() << "Duplicate";
            }
        }
    }
}

void ActivityModel::setMonth(int year, int month)
{
    QDate date;
    date.setDate(year, month + 1, 1);
    date.setDate(date.year(), date.month(), date.daysInMonth());

    if(date > QDate::currentDate())
        date = QDate::currentDate();

    emit newSearch(date);
}

int ActivityModel::getDateIndex(int year, int month)
{

    QDate date;
    date.setDate(year, month + 1, 1);

    date.setDate(date.year(), date.month(), date.daysInMonth());

    QList<QDate> keys = map.keys();

    QSet<QDate> set = QSet<QDate>::fromList(keys);
    keys = set.toList();
    QList<QDate>::const_iterator it = qUpperBound(keys, date);


    int row = it - keys.constBegin();
    qDebug() << "ROW ROW" << row;

    if(row >= rowCount())
        row = rowCount();

    return row;
}

void ActivityModel::listingFinished()
{

}
