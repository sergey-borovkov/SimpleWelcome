#include "activityset.h"
#include "itemmodel.h"
#include "localdayitem.h"
#include "localdaymodel.h"

#include <QtCore/QVariant>

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

QString LocalDayItem::id() const
{
    return "";
}

QVariant LocalDayItem::data(int role) const
{
    if(role == CurrentDateRole) {
        return getDate();
    } else if(role == ItemsRole) {
        return 0;
    } else if(role == TypesRole) {
        return types();
    } else if(role == ItemsCountRole) {
        return getCount();
    }
    return QVariant();
}

bool LocalDayItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    return false;
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

QDate LocalDayItem::getDate() const
{
    return m_date;
}

int LocalDayItem::getCount() const
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

QHash<int, QByteArray> LocalDayItem::roleNames()
{
    QHash<int, QByteArray> hash;
    hash.insert(ItemsRole, "items");
    hash.insert(CurrentDateRole, "date");
    hash.insert(CountRole, "count");
    hash.insert(TypesRole, "type");
    hash.insert(ItemsCountRole, "size");
    return hash;
}

QString LocalDayItem::types() const
{
    return m_types;
}
