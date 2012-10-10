#include "timescalemodel.h"

#include <QtCore/QModelIndex>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

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
    if (filter == "Local")
        filterRegExp = QRegExp("Image|Video|Doc");
    else if (filter == "Images")
        filterRegExp = QRegExp("Image");
    else if (filter == "Video")
        filterRegExp = QRegExp("Video");
    else if (filter == "Documents")
        filterRegExp = QRegExp("Document");
    else if (filter == "Social")
        filterRegExp = QRegExp("VKontakte|Facebook|Twitter");
    else if (filter == "VKontakte")
        filterRegExp = QRegExp("VKontakte");
    else if (filter == "Facebook")
        filterRegExp = QRegExp("Facebook");

    setFilterRegExp(filterRegExp);
}

bool TimeScaleFilterModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
    int leftYear = sourceModel()->data(left, TimeScaleItem::YearRole).toInt();
    int rightYear = sourceModel()->data(right, TimeScaleItem::YearRole).toInt();
    if (leftYear < rightYear)
        return true;
    else if (leftYear > rightYear) {
        return false;
    } else {
        int leftMonth = sourceModel()->data(left, TimeScaleItem::MonthRole).toInt();
        int rightMonth = sourceModel()->data(right, TimeScaleItem::MonthRole).toInt();
        if (leftMonth < rightMonth)
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
    QObject(parent) ,  m_year(year) , m_month(month) , m_type(type + ";")
{

}

QString TimeScaleItem::id() const
{
    return "";
}


QHash<int, QByteArray> TimeScaleItem::roleNames()
{
    QHash<int, QByteArray> names;
    names[YearRole]  = "year";
    names[MonthRole] = "month";
    names[TypesRole] = "type";
    return names;
}

QVariant TimeScaleItem::data(int role) const
{
    switch (role) {
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
    m_type += (type + ";");
    emit dataChanged();
}

void TimeScaleItem::setType(QString types)
{
    m_type = types;
    emit dataChanged();
}

TimeScaleModel::TimeScaleModel(QHash<int, QByteArray> roles, QObject *parent) :
    ListModel(roles, parent)
{
}

void TimeScaleModel::newItem(int year, int month, QString type)
{
    TimeScaleItem* item = 0;
    if ((item = find(year, month))) { // add new type to existing item
        if (!item->types().contains(type)) {
            item->addType(type);
        }
    } else { //add new item
        TimeScaleItem* item = new TimeScaleItem(year, month, type, this);
        connect(item, SIGNAL(dataChanged()), SLOT(handleItemChange()));
        appendRow(item);
    }
}

void TimeScaleModel::removeItems(const QString &type)
{
    for (int i = 0; i < rowCount(); i++) {
        QStringList types = data(index(i), TimeScaleItem::TypesRole).toString().split(";");
        int ind = types.indexOf(type);
        if (ind != -1 && types.size() == 1) {
            removeRow(i);
            i--;
        } else if (ind != -1) {
            types.removeAt(ind);
            static_cast<TimeScaleItem *>(itemAt(i))->setType(types.join(";"));
        }
    }
}

void TimeScaleModel::handleItemChange()
{
    TimeScaleItem* item = static_cast<TimeScaleItem*>(sender());
    QModelIndex index = indexFromItem(item);
    if (index.isValid())
        emit dataChanged(index, index);
}

TimeScaleItem * TimeScaleModel::find(const int year, const int month)
{
    int size = rowCount();
    for (int i = 0; i < size; i++) {
        TimeScaleItem *item = static_cast<TimeScaleItem *>(itemAt(i));
        if ((item->year() == year) && (item->month() == month))
            return item;
    }
    return 0;
}
