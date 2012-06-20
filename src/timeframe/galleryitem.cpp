#include "galleryitem.h"
#include "itemmodel.h"
#include <QVariant>
#include <QDebug>

GalleryItem::GalleryItem(const QDate &date, QObject *parent) :
    QObject(parent)
{
    m_model = new ItemModel(this);
    m_date = date;    
}

GalleryItem::~GalleryItem()
{    
    qDebug() << "delete item";
    delete m_model;
}


void GalleryItem::setDate(const QDate &d)
{
    m_date = d;
    emit dataChanged();
}

void GalleryItem::addActivity(Activity* item)
{
    m_model->addActivityItem(item);
    emit dataChanged();
}

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
