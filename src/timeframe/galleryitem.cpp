#include "galleryitem.h"
#include "itemmodel.h"
#include "activityset.h"
#include <QVariant>
//#include <QSortFilterProxyModel>
//#include <QStringList>
#include <QDebug>

TimeFrameFilterModel::TimeFrameFilterModel(QObject * parent) :
    QSortFilterProxyModel(parent)
{

}

void TimeFrameFilterModel::setSourceModel ( ItemModel * sourceModel )
{
    if (sourceModel)
    {
        QSortFilterProxyModel::setSourceModel(sourceModel);
        connect(sourceModel, SIGNAL(gotThumbnail()), this, SIGNAL(gotThumbnail()));
    }
}
QString TimeFrameFilterModel::url(int row)
{
    return data(index(row,0),ItemModel::UrlRole).toString();
}


GalleryItem::GalleryItem(const QDate &date, QObject *parent) :
    QObject(parent), m_model(0), m_date(date)
{
    m_itemModel = new ItemModel(this);
    m_model = new TimeFrameFilterModel(this);
    m_model->setSourceModel(m_itemModel);
    m_model->setDynamicSortFilter(true);
    m_model->setFilterRole(ItemModel::TypeRole);
    //m_model->setFilterFixedString("Video");
    //m_model->setFilterRegExp("Video");
}

GalleryItem::~GalleryItem()
{
    qDebug() << "delete item";
    //delete m_model;
}


void GalleryItem::setDate(const QDate &d)
{
    m_date = d;
    emit dataChanged();
}

void GalleryItem::addActivity(Activity* item)
{
    if (!m_types.contains(item->getType()))
    {
        m_types.append(item->getType());        
    }
    //ItemModel* model = qobject_cast<ItemModel*>(m_model->sourceModel());
    m_itemModel->addActivityItem(item);
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

TimeFrameFilterModel * GalleryItem::model()
{
    return m_model;
}

void GalleryItem::thumbnailReady(QString url)
{
//    if (!m_model)
//        return;

//    m_model->sourceModel();

//    ItemModel* model = qobject_cast<ItemModel*>(m_model->sourceModel());
//    if (model)
    m_itemModel->thumbnailReady(url);

}

QStringList GalleryItem::types() const
{
    return m_types;
}

void GalleryItem::setActivityFilter(const QRegExp& filter)
{
   m_model->setFilterRegExp(filter);
   emit dataChanged();
}
