#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QSet>
#include <QDate>

class ActivityProxy;
class ActivityList;
class ActivitySet;

class ActivityModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole,
        CompletedRole // true if list is full or month is already scanned
    };

    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        Q_UNUSED(column)
        Q_UNUSED(parent)
        return createIndex(row, 0);
    }

    QModelIndex parent(const QModelIndex &child) const
    {
        Q_UNUSED(child)
        return QModelIndex();
    }

    int columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)
        return 0;
    }


    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void addProxy(ActivityProxy *proxy);

public slots:
    void newActivitySet(ActivitySet *set);
    void newMonth(int year, int month);
    void monthFinished(QDate date);
    void isComplete();
    QDate getDateOfIndex(int listIndex);
    bool isNullItem(int listIndex);
    int getListIndex(int year, int month, bool direction);


private:
    QList< ActivityList *> m_list;
    QHash<int, QByteArray> hash;
    QSet <QString> m_dateSet;
    int dummy;
    // ugly, need to change later. stores locations of images paths in ActivityList
    QHash< QString, QPair<int, int> > imageLocation;
};

#endif // ACTIVITYMODEL_H
