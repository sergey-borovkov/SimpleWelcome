#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "galleryitem.h"
#include "itemmodel.h"

class ItemModel;
//class GalleryItem;
class Activity;
class GalleryLister;

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum
    {
        CurrentDateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        ItemsCountRole
    };
    explicit GalleryModel(QObject *parent = 0);
     ~GalleryModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void appendRow(GalleryItem* item);
    void appendRows(const QList<GalleryItem*> &items);
    void insertRow(int row, GalleryItem *item);
    bool removeRow(int row, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    void setLister(GalleryLister* lister);
    QModelIndex indexFromItem(const GalleryItem *item) const;
    GalleryItem * find(const QDate &date) const;
    void clear();

signals:
    
public slots:    
    void newActivities(QList <Activity*> list);
    QObject* itemsModel(QDate date) const;

protected:
    bool removeNullItem(int, int);

private slots:
    void newMonth(int, int);
    void handleItemChange();

private:
    QList <GalleryItem *> m_items;
    QHash<int, QByteArray> hash;
    GalleryLister* m_lister;
    //QList <Activity *> m_items;
};

#endif // GALLERYMODEL_H
