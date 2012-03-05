#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>
#include <QMultiMap>
#include <QMetaType>

#include <QPair>


class ActivitySet;
class ActivitySource;
class Activity;

uint qHash(const QDate &date);

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();
    QVariant data(const QModelIndex &index, int role) const;
    void addSource(ActivitySource *source);

signals:
    void newSearch(QDate);
    void newSet(int index, ActivitySet *set);

public slots:

    int rowCount(const QModelIndex &parent) const;
    void addActivities(QList<Activity *> list);
    int count() { return rowCount( QModelIndex()); }
    void setMonth(int year, int month);
    int getDateIndex(int year, int month);

private slots:
    void listingFinished();

private:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole
    };

    ActivitySource *source;

    QDate currentDate;
    QDate lastSearchedDate;

    QList < QPair <QDate, QList <Activity *> > > activities;

};

#endif // ACTIVITYMODEL_H
