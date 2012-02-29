#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDate>
#include <QMultiMap>
#include <QMetaType>
#include <QVariantList>

class ActivitySet;
class ActivitySource;
class Activity;

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();
    QVariant data(const QModelIndex &index, int role) const;
    void addSource(ActivitySource *source);

signals:
    void newSearch(QDate, QDate);
public slots:

    int rowCount(const QModelIndex &parent) const;
    void addActivities(QList<Activity *> list);
    int count() { return rowCount( QModelIndex()); }

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

    QMultiMap< QDate, Activity*> map;
    int days;
};

#endif // ACTIVITYMODEL_H
