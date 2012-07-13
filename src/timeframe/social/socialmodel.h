#ifndef SOCIALMODEL_H
#define SOCIALMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

class SocialItem;

class SocialModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SocialModel(QObject* parent = 0);
    ~SocialModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(SocialItem* item);
    void appendRows(const QList<SocialItem*> &items);
    void insertRow(int row, SocialItem* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    SocialItem* takeRow(int row);
    SocialItem* find(const QString &id) const;
    QModelIndex indexFromItem( const SocialItem* item) const;
    void clear();

private slots:
    void handleItemChange();

private:
    QList<SocialItem*> m_list;
    QHash<int, QByteArray> m_roles;
};


#endif // SOCIALMODEL_H
