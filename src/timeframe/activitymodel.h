#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractItemModel>
#include <QHash>

class ActivityProxy;
class ActivityList;
class ActivitySet;

#include <QMessageBox>
#include "activitylist.h"
class ActivityModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole,
        DummyRole
    };

    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        Q_UNUSED(column)
        Q_UNUSED(parent)
        return createIndex(row, 0);
    }

    /*bool setData(const QModelIndex &ind, const QVariant &value, int role)
    {
        QMessageBox::information(0, "dummy","dummy");
        if(role != DummyRole)
            return false;

        if(role == ActivitiesRole)
        {
            ActivityList *l = qvariant_cast<ActivityList *>(value);
            if(!l)
                m_list[ind.row()] = new ActivityList();
            else
                m_list[ind.row()] = l;
        }

        emit dataChanged(index(ind.row(), 0, QModelIndex()), index(ind.row() + 1, 0, QModelIndex()));
        dummy += 100500;
        return true;
    }*/

    QModelIndex parent(const QModelIndex &child) const
    {
        Q_UNUSED(child)
        return QModelIndex();
    }

    int columnCount(const QModelIndex &parent) const
    {
        return 0;
    }

    Qt::ItemFlags flags(const QModelIndex & index ) const
    {
            return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    void addProxy(ActivityProxy *proxy);

public slots:
    void newActivitySet(ActivitySet *set);
    void newMonth(int year, int month);

signals:
    void rowChanged(int row);
    void newRow(int row);

private:
    QList< ActivityList *> m_list;
    QHash<int, QByteArray> hash;
    int dummy;
    // ugly, need to change later. stores locations of images paths in ActivityList
    QHash< QString, QPair<int, int> > imageLocation;
};

#endif // ACTIVITYMODEL_H
