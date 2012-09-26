#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QMetaType>

#include "timeframelib_global.h"

class ListItem;

class TIMEFRAMELIB_EXPORT ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ListModel(QHash<int, QByteArray> roles, QObject* parent = 0);
    ~ListModel();
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void appendRow(ListItem* item);
    virtual void appendRows(const QList<ListItem*> &items);
    void insertRow(int row, ListItem* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void refreshRow(int row);
    ListItem* takeRow(int row);
    ListItem* find(const QString &id) const;
    ListItem* itemAt(int row) const;
    QModelIndex indexFromItem(const ListItem* item) const;
    void clear();
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
private slots:
    void handleItemChange();

private:
    QList<ListItem *> m_items;
    QHash<int, QByteArray> m_roles;
};

Q_DECLARE_METATYPE( ListModel* )

#endif // LISTMODEL_H


