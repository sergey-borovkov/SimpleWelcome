#include "appsgridmodel.h"
#include <kservicegroup.h>
#include <kdebug.h>

AppsGridModel::AppsGridModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names;
    names[ CaptionRole ] = "caption";
    names[ ImagePathRole ] = "imagePath";
    setRoleNames(names);
}

QList<AppItem> GetFlatList(QString group)
{
    QList<AppItem> out;

    KServiceGroup::Ptr root = KServiceGroup::group(group);

    if (!root || !root->isValid())
        return out;

    const KServiceGroup::List list = root->entries(true /* sorted */,
                                                   true /* exclude no display entries */,
                                                   false /* allow separators */,
                                                   true /* sort by generic name */);

    for (KServiceGroup::List::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it)
    {
        const KSycocaEntry::Ptr p = (*it);

        if (p->isType(KST_KService))
        {
            const KService::Ptr service = KService::Ptr::staticCast(p);

            if (service->noDisplay())
                continue;

            AppItem newItem;
            newItem.icon = service->icon();
            newItem.caption = service->name();
            newItem.desktopEntry = service->entryPath();
            out.append(newItem);
        }
        else if (p->isType(KST_KServiceGroup))
        {
            const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

            if (serviceGroup->noDisplay() || serviceGroup->childCount() == 0)
                continue;

            if (serviceGroup->relPath().indexOf("/") != serviceGroup->relPath().size() - 1)
                out.append(GetFlatList(serviceGroup->relPath()));
            else
            {
                AppItem newItem;
                newItem.icon = serviceGroup->icon();
                newItem.caption = serviceGroup->caption();
                newItem.relPath = serviceGroup->relPath();
                out.append(newItem);
            }
        }
        else
        {
            kDebug(250) << "KServiceGroup: Unexpected object in list!";
            continue;
        }
    }
    return out;
}

QList<AppItem> AppsGridModel::GetList(QString currentGroup) const
{
    static QList<AppItem> out;
    static QString prevCurrentGroup = "-1";
    if (prevCurrentGroup == currentGroup && !out.isEmpty())
        return out;

    prevCurrentGroup = currentGroup;
    out = GetFlatList(currentGroup);

    for (int i = 0; i < out.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(out[i].icon), out[i].caption);

    return out;
}

int AppsGridModel::rowCount( const QModelIndex & /*parent*/ ) const
{
    return GetList(currentGroup).size();
}

QVariant AppsGridModel::data( const QModelIndex &index, int role ) const
{
    QList<AppItem> list = GetList(currentGroup);

    if ( index.row() < 0 || index.row() >= list.size() )
        return QVariant();

    switch (role)
    {
    case CaptionRole:
        return list[index.row()].caption;
    case ImagePathRole:
        return QString("image://generalicon/appicon/%1").arg(list[index.row()].icon);
    }

    return QVariant();
}

#include <QMessageBox>

void AppsGridModel::itemClicked(int newIndex)
{
    if (currentGroup == "" && newIndex == -1)
        return;

    if (newIndex != -1)
    {
        AppItem clickedItem = GetList(currentGroup)[newIndex];
        if (clickedItem.relPath.isEmpty())
        {
            QMessageBox::information(0, clickedItem.desktopEntry, clickedItem.caption);
            return;
        }
    }


    emit dataClear();

    int rowCountWas = rowCount();
    beginRemoveRows(QModelIndex(), 0, rowCountWas);
    endRemoveRows();

    if (newIndex == -1)
        currentGroup = "";
    else
    {
        AppItem clickedItem = GetList(currentGroup)[newIndex];
        currentGroup = clickedItem.relPath;
    }

    int rowCountNew = rowCount();
    beginInsertRows(QModelIndex(), 0, rowCountNew);
    endInsertRows();

    emit contentChanged();
}

void AppsGridModel::updateContent()
{
    GetList(currentGroup);
}
