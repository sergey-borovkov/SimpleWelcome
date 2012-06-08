#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractListModel>
#include <QMetaType>

class Activity;

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole,
        UrlRole,
        TypeRole
    };
    explicit ItemModel(QObject *parent = 0);    
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    
signals:
    
public slots:
    
private:
    QHash<int, QByteArray> m_hash;
    QList<Activity *> m_items;
};

Q_DECLARE_METATYPE(ItemModel *)

#endif // ITEMMODEL_H
