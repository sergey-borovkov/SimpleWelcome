#include "activityproxy.h"
#include "activityset.h"
#include "itemmodel.h"
#include "localdayitem.h"
#include "localcontentmodel.h"

#include <QtCore/QRegExp>
#include <QtCore/QVariant>
#include <QtGui//QSortFilterProxyModel>


LocalContentFilterModel::LocalContentFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(LocalDayItem::TypesRole);
}

void LocalContentFilterModel::setFilter(const QString &filter)
{
    QRegExp filterRegExp;
    if (filter == "Local")
        filterRegExp = QRegExp("Image|Video|Doc");
    else if (filter == "Photo")
        filterRegExp = QRegExp("Image");
    else if (filter == "Video")
        filterRegExp = QRegExp("Video");
    else if (filter == "Documents")
        filterRegExp = QRegExp("Document");
    setFilterRegExp(filterRegExp);
    LocalContentModel *model = qobject_cast<LocalContentModel *>(sourceModel());
    if (model)
        model->setFilter(filterRegExp);
    for (int i = 0; i < rowCount(); i++) { //Set filter on nested models
        QDate date = data(index(i, 0), LocalDayItem::CurrentDateRole).toDate();
        TimeFrameFilterModel * tModel = qobject_cast<TimeFrameFilterModel *> (itemsModel(date));
        if (tModel)
            tModel->setFilterRegExp(filterRegExp);
    }

}

QObject* LocalContentFilterModel::itemsModel(QDate date) const
{
    LocalContentModel* model = qobject_cast<LocalContentModel*>(sourceModel());
    if (model)
        return model->itemsModel(date);
    return 0;
}

QDate LocalContentFilterModel::getDateOfIndex(int listIndex)
{
    if ((listIndex >= rowCount()) || (listIndex < 0))
        return QDate();
    return data(index(listIndex, 0), LocalDayItem::CurrentDateRole).toDate();
}

int LocalContentFilterModel::getIndexByDate(int year, int month, bool direction)
{
    Q_UNUSED(direction)
    for (int i = 0; i < rowCount(); i++) {
        QDate date = data(index(i, 0), LocalDayItem::CurrentDateRole).toDate();
        if ((date.year() == year) && (date.month() == month))
            return i;
    }
    return -1;
}


LocalContentModel::LocalContentModel(QHash<int, QByteArray> roles, QObject* parent) :
    ListModel(roles, parent),
    m_lister(0)
{
}


QVariant LocalContentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == LocalDayItem::CountRole) {
        return rowCount();
    }

    return ListModel::data(index, role);
}

QObject* LocalContentModel::itemsModel(QDate date) const
{
    for (int i = 0; i < rowCount(); i++) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(i));
        if (date == item->getDate())
            return item->model();
    }
    return 0;
}

void LocalContentModel::setLister(ActivityProxy *lister)
{
    if (m_lister)
        delete m_lister;
    m_lister = lister;
    m_lister->setModel(this);
    connect(m_lister, SIGNAL(newActivities(QList<Activity*>)), SLOT(newActivities(QList<Activity*>)));
}

void LocalContentModel::newActivities(QList<Activity*> list)
{
    for (int i = 0; i < list.size() ; i++) {

        Activity* newItem = list.at(i);
        if (m_urlHash.contains(newItem->getUrl()))
            continue;
        m_urlHash.insert(newItem->getUrl(), newItem->getDate());

        //first check of null item, if we find one, edit it with new data
        for (int j = 0; j < rowCount(); j++) {
            LocalDayItem *it = static_cast<LocalDayItem *>(itemAt(j));
            if ((it->getDate().year() == newItem->getDate().year())
                    && (it->getDate().month() == newItem->getDate().month())
                    && (it->getCount() == 0)
               ) {
                it->setDate(newItem->getDate());
                it->addActivity(newItem);
                continue;
            }
        }

        int j = 0;
        bool flag = false;
        if (rowCount() > 0) {
            LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(0));
            while (item->getDate() <= newItem->getDate()) {
                if (item->getDate() == newItem->getDate()) {
                    item->addActivity(newItem);
                    flag = true;
                    break;
                }
                j++;
                if (j == rowCount()) {
                    break;
                }
                item = static_cast<LocalDayItem *>(itemAt(j));
            }
        }

        if (flag) {
            continue;
        }

        LocalDayItem * gallItem = new LocalDayItem(newItem->getDate(), this);
        gallItem->addActivity(newItem);
        insertRow(j, gallItem);
    }
}

//Add null gallery item to model
void LocalContentModel::newMonth(int year, int month)
{
    QDate date(year, month, 1);
    for (int i = 0; i < rowCount(); i++) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(i));
        if ((item->getDate().year() == year) && (item->getDate().month() == month)) {
            return;
        }
    }

    int j = 0;
    LocalDayItem * gallItem = new LocalDayItem(date, this);
    if (rowCount() > 0) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(0));
        while (item->getDate() <= gallItem->getDate()) {
            j++;
            if (j == rowCount()) {
                break;
            }
            item = static_cast<LocalDayItem *>(itemAt(j));
        }
    }

    insertRow(j, gallItem);
}

//Remove null gallery item from model
bool LocalContentModel::removeNullItem(int year, int month)
{
    QDate date(year, month, 1);
    int j = 0;
    if (rowCount() > 0) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(0));
        while (item->getDate() <= date) {
            if (item->getDate() == date) {
                if (item->model()->rowCount(QModelIndex()) == 0) {
                    removeRow(j);
                    return true;
                }
            }
            j++;
            if (j == rowCount()) {
                break;
            }
            item = static_cast<LocalDayItem *>(itemAt(j));
        }
    }
    return false;
}


void LocalContentModel::appendRow(LocalDayItem *item)
{
    appendRows(QList<LocalDayItem*>() << item);
}

void LocalContentModel::appendRows(const QList<LocalDayItem *> &items)
{
    QList<ListItem *> list;
    foreach(LocalDayItem * item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        list += item;
    }
    ListModel::appendRows(list);
}

void LocalContentModel::insertRow(int row, LocalDayItem *item)
{
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    ListModel::insertRow(row, item);
}

void LocalContentModel::handleItemChange()
{
    LocalDayItem* item = static_cast<LocalDayItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if (index.isValid())
        emit dataChanged(index, index);
}

LocalDayItem * LocalContentModel::find(const QDate &date) const
{
    const int size = rowCount();
    for (int i = 0; i < size; i++) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(i));
        if (item->getDate() == date)
            return item;
    }

    return 0;
}

void LocalContentModel::setFilter(QRegExp regexp)
{
    m_filter = regexp;
}

QRegExp LocalContentModel::filter() const
{
    return m_filter;
}

int LocalContentModel::getIndexByDate(int year, int month,  bool direction)
{
    Q_UNUSED(direction)

    LocalDayItem* ptr = 0;

    for (int i = 0; i < rowCount(); i++) {
        LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(i));
        if ((item->getDate().year() == year) && (item->getDate().month() == month)) {
            ptr = item;
            break;
        }
    }
    if (ptr)
        return indexFromItem(ptr).row();
    return -1;
}


QDate LocalContentModel::getDateOfIndex(int listIndex)
{
    if (listIndex >= rowCount() || listIndex < 0)
        return QDate();
    LocalDayItem *item = static_cast<LocalDayItem *>(itemAt(listIndex));
    return item->getDate();
}

void LocalContentModel::imageReady(QString url)
{
    if (m_urlHash.contains(url)) {
        QDate date = m_urlHash[url];
        LocalDayItem* item = find(date);
        if (item)
            item->thumbnailReady(url);
    }
}
