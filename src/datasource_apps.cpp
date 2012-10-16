#include "datasource_apps.h"
#include <KServiceGroup>
#include <KConfigGroup>
#include "datasource_recentapps.h"

AppItemList GetFlatList(QString group)
{
    AppItemList out;

    KServiceGroup::Ptr root = KServiceGroup::group(group);

    if (!root || !root->isValid())
        return out;

    const KServiceGroup::List list = root->entries(true /* sorted */,
                                     true /* exclude no display entries */,
                                     false /* allow separators */,
                                     true /* sort by generic name */);

    for (KServiceGroup::List::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it) {
        const KSycocaEntry::Ptr p = (*it);

        if (p->isType(KST_KService)) {
            const KService::Ptr service = KService::Ptr::staticCast(p);

            if (service->noDisplay())
                continue;

            AppItem newItem;

            newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(service->icon());
            newItem["caption"] = service->name();
            newItem["id"] = out.count();
            newItem["desktopEntry"] = service->entryPath();
            newItem["group"] = group;

            out.append(newItem);
        } else if (p->isType(KST_KServiceGroup)) {
            const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

            if (serviceGroup->noDisplay() || serviceGroup->childCount() == 0)
                continue;

            //if (serviceGroup->relPath().indexOf("/") != serviceGroup->relPath().size() - 1)
            out.append(GetFlatList(serviceGroup->relPath()));
            /*else
            {
                AppItem newItem;
                newItem.icon = QString("image://generalicon/appicon/%1").arg(serviceGroup->icon());
                newItem.caption = serviceGroup->caption();
                newItem.relPath = serviceGroup->relPath();
                out.append(newItem);
            }*/
        } else {
            qDebug() << "KServiceGroup: Unexpected object in list!";
            continue;
        }
    }
    return out;
}

DataSource_Apps::DataSource_Apps(QObject *parent, DataSource_RecentApps *inRecentApps)
    : DataSource(parent), prevCurrentGroup("-1"), recentApps(inRecentApps)
{
    updateItems(false);
}

int DataSource_Apps::getItemCount()
{
    return appsList.count();
}


QString DataSource_Apps::itemUrlDnd(int id)
{
    if (id < 0 || id >= appsList.count())
        return QString();
    return QString("file://%1").arg(appsList[id]["desktopEntry"].toString());
}

void DataSource_Apps::getContent()
{
    for (int i = 0; i < appsList.size(); i++) {
        emit newItemData(appsList[i], qmlGroupName);
    }
}

void DataSource_Apps::updateItems(bool isResetContent/* = true*/)
{
    prevCurrentGroup = currentGroup;

    AppItemList newList = GetFlatList(currentGroup);
    qSort(newList);

    if (newList != appsList) {
        appsList = newList;

        if (isResetContent)
            emit resetContent();
    }
}

void DataSource_Apps::itemClicked(int newIndex)
{
    if (currentGroup == "" && newIndex == -1)
        return;

    if (newIndex != -1) {
        AppItem clickedItem = appsList[newIndex];
        if (clickedItem["relPath"].toString().isEmpty()) {
            recentApps->addRecentApp(clickedItem["desktopEntry"].toString());
            emit runDesktopFile(clickedItem["desktopEntry"].toString());
            return;
        }
    }

    if (newIndex == -1)
        currentGroup = "";
    else
        currentGroup = appsList[newIndex]["relPath"].toString();

    if (prevCurrentGroup != currentGroup)
        updateItems(false);

    emit resetContent();
}
