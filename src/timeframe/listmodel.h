#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>

class ListItem;

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ListModel(QHash<int, QByteArray> roles, QObject* parent = 0);
    ~ListModel();    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(ListItem* item);
    virtual void appendRows(const QList<ListItem*> &items);
    void insertRow(int row, ListItem* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void refreshRow(int row);
    ListItem* takeRow(int row);
    ListItem* find(const QString &id) const;
    QModelIndex indexFromItem(const ListItem* item) const;
    void clear();
private slots:
    void handleItemChange();

private:
    QList<ListItem *> m_items;
    QHash<int, QByteArray> m_roles;
};

#endif // LISTMODEL_H
