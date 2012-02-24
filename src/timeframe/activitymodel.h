#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>

class ActivitySet;
class ActivitySource;

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void addSource(ActivitySource *source);
signals:
    
public slots:
    void addActivitySet(ActivitySet *set);

private:
    ActivitySource *source;

    // temporary, will have to change it later to more efficient data structure
    QList<ActivitySet *> activities;
    QDate currentDate;
};

#endif // ACTIVITYMODEL_H
