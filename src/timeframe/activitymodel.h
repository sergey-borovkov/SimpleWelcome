#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>
#include <QMultiMap>
#include <QMetaType>

class ActivitySet;
class ActivitySource;
class Activity;

Q_DECLARE_METATYPE(QList<Activity *>)

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();
    QVariant data(const QModelIndex &index, int role) const;
    void addSource(ActivitySource *source);

signals:
    
public slots:

    int rowCount(const QModelIndex &parent) const;
    void addActivitySet(ActivitySet *set);
    void addActivities(QList<Activity *> list);
    int count() { return rowCount( QModelIndex()); }

private:
    static const int CurrentDateRole;
    static const int ActivitiesRole;

    ActivitySource *source;

    // temporary, will have to change it later to more efficient data structure
    QList<ActivitySet *> activities;
    QDate currentDate;
    QMultiMap< QDate, Activity*> map;
};

#endif // ACTIVITYMODEL_H
