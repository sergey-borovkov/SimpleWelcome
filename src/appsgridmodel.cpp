#include "appsgridmodel.h"
#include "appprovider.h"
#include <kservicegroup.h>

AppsGridModel::AppsGridModel(AppProvider *appProvider, QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names;
    names[ CaptionRole ] = "caption";
    names[ ImagePathRole ] = "imagePath";
    setRoleNames(names);

    this->appProvider = appProvider;
}

AppsGridModel::~AppsGridModel()
{
}

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;
};

QList<AppItem> GetList(QString currentGroup)
{
    QList<AppItem> out;

    QString _relPath;
    KServiceGroup::Ptr root = KServiceGroup::group(currentGroup);

    if (!root || !root->isValid()) {
        return out;
    }

    const KServiceGroup::List list = root->entries(true /* sorted */,
                                                   true /* exclude no display entries */,
                                                   false /* allow separators */,
                                                   true /* sort by generic name */);

    for (KServiceGroup::List::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it)
    {
        const KSycocaEntry::Ptr p = (*it);
        AppItem newItem;

        if (p->isType(KST_KService))
        {
            const KService::Ptr service = KService::Ptr::staticCast(p);

            if (service->noDisplay())
                continue;

            newItem.icon = service->icon();
            newItem.caption = service->name();
            newItem.desktopEntry = service->entryPath();
        }
        else if (p->isType(KST_KServiceGroup))
        {
            const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

            if (serviceGroup->noDisplay() || serviceGroup->childCount() == 0) {
                continue;
            }

            newItem.icon = serviceGroup->icon();
            newItem.relPath = serviceGroup->relPath();
            //if (iconNameMap().contains(icon))
            //{
            //    newItem.icon = iconNameMap().value(icon);
            //}

            //desktopEntry = serviceGroup->entryPath();
            newItem.caption = serviceGroup->caption();
            //relPath = serviceGroup->relPath();
            // newItem.caption = serviceGroup->comment();
        } else {
            kWarning(250) << "KServiceGroup: Unexpected object in list!";
            continue;
        }
        out.append(newItem);
    }
    return out;
}

int AppsGridModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )

    KServiceGroup::Ptr group = KServiceGroup::root();
    if (!group || !group->isValid())
        return 0;

    KServiceGroup::List list = group->entries();

    QList<AppItem> lst = GetList(currentGroup);

    return lst.size(); //appProvider->getRootGroups().size();
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

    return 5;
}

bool AppsGridModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( !index.isValid() )
        return false;

    return true;
}

#include <QMessageBox>

void AppsGridModel::changeFolder(int newIndex)
{
    if (newIndex != -1)
    {
        AppItem clickedItem = GetList(currentGroup)[newIndex];
        if (clickedItem.relPath.isEmpty())
        {
            QMessageBox::information(0, clickedItem.desktopEntry, clickedItem.caption);
            return;
        }
    }

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
}
