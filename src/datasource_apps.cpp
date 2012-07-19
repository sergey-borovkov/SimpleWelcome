#include "datasource_apps.h"
#include <kservicegroup.h>
#include <kdebug.h>

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

void DataSource_Apps::updateContent()
{
    if (prevCurrentGroup == currentGroup && !appsList.isEmpty())
        return;

    prevCurrentGroup = currentGroup;
    appsList = GetFlatList(currentGroup);

    for (int i = 0; i < appsList.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(appsList[i].icon), appsList[i].caption);
}

#include <QMessageBox>

void DataSource_Apps::itemClicked(int newIndex)
{
    if (currentGroup == "" && newIndex == -1)
        return;

    if (newIndex != -1)
    {
        AppItem clickedItem = appsList[newIndex];
        if (clickedItem.relPath.isEmpty())
        {
            QMessageBox::information(0, clickedItem.desktopEntry, clickedItem.caption);
            return;
        }
    }

    emit dataClear();

    if (newIndex == -1)
        currentGroup = "";
    else
        currentGroup = appsList[newIndex].relPath;

    updateContent();
}
