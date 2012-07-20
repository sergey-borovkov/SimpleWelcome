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

public slots:
    void newActivities(QList <Activity*> list);
    QObject* itemsModel(QDate date) const;
    int getIndexByDate(int year, int month, bool direction);
    QDate getDateOfIndex(int listIndex);
    void imageReady(QString url);
    void setActivityType(const QString& type);

protected:
    bool removeNullItem(int, int);

private slots:
    void newMonth(int, int);
    void handleItemChange();

private:
    QList <GalleryItem *> m_items;
    QHash<int, QByteArray> hash;
    QHash<QString, QDate> m_urlHash;
    GalleryLister* m_lister;
};

#endif // GALLERYMODEL_H
