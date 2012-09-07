#include "localdayitem.h"
#include "localdaymodel.h"
#include "itemmodel.h"
#include "activityset.h"
#include <QVariant>

TimeFrameFilterModel::TimeFrameFilterModel(QObject * parent) :
    QSortFilterProxyModel(parent)
{

}

void TimeFrameFilterModel::setSourceModel(ItemModel * sourceModel)
{
    if(sourceModel) {
        QSortFilterProxyModel::setSourceModel(sourceModel);
        connect(sourceModel, SIGNAL(gotThumbnail()), this, SIGNAL(gotThumbnail()));
    }
}
QString TimeFrameFilterModel::url(int row)
{
    return data(index(row, 0), ItemModel::UrlRole).toString();
}


LocalDayItem::LocalDayItem(const QDate &date, QObject *parent) :
    QObject(parent), m_date(date), m_model(0)
{
    m_itemModel = new ItemModel(this);
    m_model = new TimeFrameFilterModel(this);
    m_model->setSourceModel(m_itemModel);
    m_model->setDynamicSortFilter(true);
    m_model->setFilterRole(ItemModel::TypeRole);

    LocalDayModel *model = qobject_cast<LocalDayModel *>(parent);
    if(model)
        m_model->setFilterRegExp(model->filter());
}

LocalDayItem::~LocalDayItem()
{
}


void LocalDayItem::setDate(const QDate &d)
{
    m_date = d;
    emit dataChanged();
}

void LocalDayItem::addActivity(Activity* item)
{
    if(!m_types.contains(item->getType())) {
        m_types += (';' + item->getType());
    }

    m_itemModel->addActivityItem(item);
    emit dataChanged();
}

QDate LocalDayItem::getDate()
{
    return m_date;
}

int LocalDayItem::getCount()
{
    return m_model->rowCount(QModelIndex());
}

TimeFrameFilterModel * LocalDayItem::model()
{
    return m_model;
}

void LocalDayItem::thumbnailReady(QString url)
{
    m_itemModel->thumbnailReady(url);

}

QString LocalDayItem::types() const
{
    return m_types;
}
