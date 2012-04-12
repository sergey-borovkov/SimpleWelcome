#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>
#include <QHash>

class ActivityProxy;
class ActivityList;
class ActivitySet;

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole
    };

    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void addProxy(ActivityProxy *proxy);

public slots:
    void onNewActivitySet(ActivitySet *set);

private:
    QList< ActivityList *> m_list;
    QHash<int, QByteArray> hash;
};

#endif // ACTIVITYMODEL_H
