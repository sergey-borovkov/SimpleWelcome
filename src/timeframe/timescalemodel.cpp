#include "timescalemodel.h"

#include <QDebug>
#include <QModelIndex>
#include <QRegExp>

TimeScaleFilterModel::TimeScaleFilterModel(QObject * parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterRole(TimeScaleItem::TypesRole);
    setSortRole(TimeScaleItem::YearRole);
    setFilterRegExp(QRegExp("Image|Video|Document"));
    sort(0);
}

/*TO-DO: add dynamic plugin type*/

void TimeScaleFilterModel::setFilter(const QString &filter)
{
    QRegExp filterRegExp;
    if(filter == "Local")
        filterRegExp = QRegExp("Image|Video|Doc");
    else if(filter == "Photo")
        filterRegExp = QRegExp("Image");
    else if(filter == "Video")
        filterRegExp = QRegExp("Video");
    else if(filter == "Documents")
        filterRegExp = QRegExp("Document");
    else if(filter == "Social")
        filterRegExp = QRegExp("VKontakte|Facebook|Twitter");
    else if(filter == "VKontakte")
        filterRegExp = QRegExp("VKontakte");
    else if(filter == "Facebook")
        filterRegExp = QRegExp("Facebook");

    setFilterRegExp(filterRegExp);
}

bool TimeScaleFilterModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
    int leftYear = sourceModel()->data(left, TimeScaleItem::YearRole).toInt();
    int rightYear = sourceModel()->data(right, TimeScaleItem::YearRole).toInt();
    if(leftYear < rightYear)
        return true;
    else if(leftYear > rightYear) {
        return false;
    } else {
        int leftMonth = sourceModel()->data(left, TimeScaleItem::MonthRole).toInt();
        int rightMonth = sourceModel()->data(right, TimeScaleItem::MonthRole).toInt();
        if(leftMonth < rightMonth)
            return true;
    }
    return false;
}
int TimeScaleFilterModel::getYear(int ind)
{
    return data(index(ind, 0), TimeScaleItem::YearRole).toInt();
}

int TimeScaleFilterModel::getMonth(int ind)
{
    return data(index(ind, 0), TimeScaleItem::MonthRole).toInt();
}

int TimeScaleFilterModel::count()
{
    return rowCount();
}

TimeScaleItem::TimeScaleItem(int year, int month, QString type, QObject *parent):
    QObject(parent) ,  m_year(year) , m_month(month) , m_type(type)
{

}


QHash<int, QByteArray> TimeScaleItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[YearRole]  = "year";
    names[MonthRole] = "month";
    names[TypesRole] = "type";
    return names;
}

QVariant TimeScaleItem::data(int role) const
{
    switch(role) {
    case YearRole:
        return year();
    case MonthRole:
        return month();
    case TypesRole:
        return types();
    default:
        return QVariant();
    }
}

int TimeScaleItem::year() const
{
    return m_year;
}

int TimeScaleItem::month() const
{
    return m_month;
}

QString TimeScaleItem::types() const
{
    return m_type;
}

void TimeScaleItem::addType(QString type)
{
    //qDebug() << type;
    if(!m_type.contains(type)) {
        m_type += (type + ";");
        emit dataChanged();
    }
}

TimeScaleModel::TimeScaleModel(TimeScaleItem* prototype, QObject *parent) :
    QAbstractListModel(parent), m_prototype(prototype)
{
    setRoleNames(m_prototype->roleNames());
}

TimeScaleModel::~TimeScaleModel()
{
    delete m_prototype;
    clear();
}

void TimeScaleModel:: newItem(int year, int month, QString type)
{
    QPair <int, int> pair;
    pair.first = year;
    pair.second = month;
    if(m_dates.contains(pair)) { // add new type to existing item
        TimeScaleItem* item = find(year, month);
        if(!item)
            return;
        if(!item->types().contains(type)) {
            item->addType(type);
        }
    } else { //add new item
        m_dates.insert(pair);
        TimeScaleItem* item = new TimeScaleItem(year, month, type, this);
        appendRow(item);
    }
}

QVariant TimeScaleModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_list.size())
        return QVariant();
    return m_list.at(index.row())->data(role);
}

int TimeScaleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.size();
}

void TimeScaleModel::appendRow(TimeScaleItem *item)
{
    appendRows(QList<TimeScaleItem*>() << item);
}

void TimeScaleModel::appendRows(const QList<TimeScaleItem *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach(TimeScaleItem * item, items) {
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        m_list.append(item);
    }
    endInsertRows();
}

void TimeScaleModel::insertRow(int row, TimeScaleItem *item)
{
    beginInsertRows(QModelIndex(), row, row);
    connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
    m_list.insert(row, item);
    endInsertRows();
}

void TimeScaleModel::handleItemChange()
{
    TimeScaleItem* item = static_cast<TimeScaleItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if(index.isValid())
        emit dataChanged(index, index);
}

TimeScaleItem * TimeScaleModel::find(const int year, const int month)
{
    foreach(TimeScaleItem * item, m_list) {
        if((item->year() == year) && (item->month() == month))
            return item;
    }
    return 0;
}

QModelIndex TimeScaleModel::indexFromItem(const TimeScaleItem *item) const
{
    Q_ASSERT(item);
    for(int row = 0; row < m_list.size(); ++row) {
        if(m_list.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void TimeScaleModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

bool TimeScaleModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size()) return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

bool TimeScaleModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || (row + count) >= m_list.size()) return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i) {
        delete m_list.takeAt(row);
    }
    endRemoveRows();
    return true;
}

TimeScaleItem * TimeScaleModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    TimeScaleItem* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}
