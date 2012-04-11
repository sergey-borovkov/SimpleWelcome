#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractListModel>

class ActivityModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ActivityModel(QObject *parent = 0);
    ~ActivityModel();

    QVariant data(const QModelIndex &index, int role) const;
    const QHash<int, QByteArray> & roleNames();

signals:
    
public slots:
    
};

#endif // ACTIVITYMODEL_H
