#include "activity.h"
#include "localdaymodel.h"

#include <QtCore/QDate>

LocalDayModel::LocalDayModel(QDate date, QObject *parent) :
    QAbstractListModel(parent), m_date(date)
{
    m_hash.insert(ActivitiesRole, "activity");
    m_hash.insert(CurrentDateRole, "date");
    m_hash.insert(CountRole, "count");
    m_hash.insert(UrlRole, "url");
    m_hash.insert(TypeRole, "type");
    setRoleNames(m_hash);
}

LocalDayModel::~LocalDayModel()
{
    qDeleteAll(m_items);
    m_items.clear();
}


QVariant LocalDayModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == CurrentDateRole) {
        return m_items.value(index.row())->date();
    } else if (role == ActivitiesRole) {
        QVariant v;
        v.setValue(m_items[index.row()]);
        return v;
    } else if (role == CountRole) {
        return m_items.size();
    } else if (role == UrlRole) {
        return m_items[index.row()]->url();
    } else if (role == TypeRole) {
        return m_items[index.row()]->type();
    }
    return QVariant();
}

int LocalDayModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_items.size();
}

void LocalDayModel::addActivityItem(Activity *item)
{
    if (!item)
        return;
    /* Check duplicates*/
    if (m_urlSet.contains(item->url()))
        return;

    int ind = m_items.size();
    beginInsertRows(QModelIndex(), ind , ind);
    m_items.append(item);
    m_urlSet.insert(item->url());
    endInsertRows();
}

/*void LocalDayModel::thumbnailReady(const QString &url)
{
    /*int row;
    for (row = 0; row < m_items.size(); ++row) {
        if (m_items.at(row)->url() == url) {
            m_items.at(row)->setUrl(url);
           break;
        }
    }
    dataChanged(index(row), index(row));

    emit gotThumbnail(url);

    //emit gotThumbnail();
}*/

QString LocalDayModel::url(int row) const
{
    return m_items.at(row)->url();
}

QDate LocalDayModel::date() const
{
    return m_date;
}

void LocalDayModel::setDate(QDate date)
{
    m_date = date;
}


LocalDayFilterModel::LocalDayFilterModel(QObject * parent) :
    QSortFilterProxyModel(parent)
{
}

void LocalDayFilterModel::setSourceModel(LocalDayModel * sourceModel)
{
    if (sourceModel) {
        QSortFilterProxyModel::setSourceModel(sourceModel);
        //connect(sourceModel, SIGNAL(gotThumbnail()), this, SIGNAL(gotThumbnail()));
    }
}

int LocalDayFilterModel::count() const
{
    return rowCount();
}

QString LocalDayFilterModel::url(int row) const
{
    return data(index(row, 0), LocalDayModel::UrlRole).toString();
}

void LocalDayFilterModel::previewReady(const QString &url)
{
    emit gotThumbnail(url);
}

