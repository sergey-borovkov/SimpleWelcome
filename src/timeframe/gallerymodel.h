#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "galleryitem.h"
#include "itemmodel.h"

class ItemModel;
//class GalleryItem;
class Activity;

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
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;

signals:
    
public slots:    
    void newActivity(Activity *item);
    QObject* itemsModel(int index) const;

private:
    QList <GalleryItem *> m_items;
    QHash<int, QByteArray> hash;
    //QList <Activity *> m_items;
};

#endif // GALLERYMODEL_H
