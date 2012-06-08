#include "galleryitem.h"
#include "itemmodel.h"
#include <QVariant>


GalleryItem::GalleryItem(const QDate &date, QObject *parent) :
    QObject(parent)
{
    m_model = new ItemModel();
    m_date = date;
}

GalleryItem::~GalleryItem()
{
    delete m_model;
}


void GalleryItem::setDate(const QDate &d)
{
    m_date = d;
}
  /* TO-DO: add activity in model*/
/*
void GalleryItem::addActivity(Activity)
{

}
*/
QDate GalleryItem::getDate()
{
    return m_date;
}

int GalleryItem::getCount()
{
    return m_model->rowCount(QModelIndex());
}

ItemModel * GalleryItem::model()
{
    return m_model;
}
