#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QObject>
#include <QMetaType>

#include <QDate>

class ItemModel;
class Activity;


class GalleryItem : public QObject
{
    Q_OBJECT
public:    
    explicit GalleryItem(const QDate &date, QObject *parent = 0);
    ~GalleryItem();

    QDate getDate();
    int   getCount();
    ItemModel * model();

signals:
    
public slots:
    void setDate(const QDate&);
    //void addActivity(Activity);

private:

    QDate      m_date;
    int        m_count;
    ItemModel *m_model;
};

#endif // GALLERYITEM_H
