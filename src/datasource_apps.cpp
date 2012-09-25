#include "datasource_apps.h"
#include <KServiceGroup>
#include <KConfigGroup>
#include "datasource_recentapps.h"

AppItemList GetFlatList(QString group)
{
    AppItemList out;

    KServiceGroup::Ptr root = KServiceGroup::group(group);

    if(!root || !root->isValid())
        return out;

    const KServiceGroup::List list = root->entries(true /* sorted */,
                                     true /* exclude no display entries */,
                                     false /* allow separators */,
                                     true /* sort by generic name */);

    for(KServiceGroup::List::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it) {
        const KSycocaEntry::Ptr p = (*it);

        if(p->isType(KST_KService)) {
            const KService::Ptr service = KService::Ptr::staticCast(p);

            if(service->noDisplay())
                continue;

            AppItem newItem;
            newItem.icon = service->icon();
            newItem.caption = service->name();
            newItem.desktopEntry = service->entryPath();
            out.append(newItem);
        } else if(p->isType(KST_KServiceGroup)) {
            const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

            if(serviceGroup->noDisplay() || serviceGroup->childCount() == 0)
                continue;

            //if (serviceGroup->relPath().indexOf("/") != serviceGroup->relPath().size() - 1)
            out.append(GetFlatList(serviceGroup->relPath()));
            /*else
            {
                AppItem newItem;
                newItem.icon = serviceGroup->icon();
                newItem.caption = serviceGroup->caption();
                newItem.relPath = serviceGroup->relPath();
                out.append(newItem);
            }*/
        }
        else
        {
            qDebug() << "KServiceGroup: Unexpected object in list!";
            continue;
        }
    }
    return out;
}

bool userAppsLessThan(const QPair<QString, int> &a, const QPair<QString, int> &b)
{
    return a.second < b.second;
}

DataSource_Apps::DataSource_Apps(QObject *parent, DataSource_RecentApps *inRecentApps)
    : DataSource(parent), prevCurrentGroup("-1"), recentApps(inRecentApps)
{
    KConfigGroup configGroup(KGlobal::config(), "General");
    QStringList appsOrderList = configGroup.readEntry("User applications order", QStringList());
    for (int i = 0; i < appsOrderList.size(); i++)
    {
        int indexOfSpace = appsOrderList[i].indexOf(" ");
        if (indexOfSpace != -1)
        {
            int index = appsOrderList[i].left(indexOfSpace).toInt();
            QString caption = appsOrderList[i].right(appsOrderList[i].size() - indexOfSpace - 1);
            userAppsOrder.append(qMakePair(caption, index));
        }
    }

    qSort(userAppsOrder.begin(), userAppsOrder.end(), userAppsLessThan);

    updateItems();
}

DataSource_Apps::~DataSource_Apps()
{
    saveData();
}

int DataSource_Apps::getItemCount()
{
    return appsList.count();
}


QString DataSource_Apps::itemUrlDnd(int id)
{
    if (id < 0 || id >= appsList.count())
        return QString();
    return QString::fromAscii("file://") + appsList[id].desktopEntry;
}

void DataSource_Apps::saveData()
{
    QStringList out;
    for (int i = 0; i < userAppsOrder.size(); i++)
        out.append(QString("%1 %2").arg(userAppsOrder[i].second).arg(userAppsOrder[i].first));

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("User applications order", out);
    configGroup.sync();
}

void DataSource_Apps::getContent()
{
    for(int i = 0; i < appsList.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(appsList[i].icon), appsList[i].caption, i);
}

void DataSource_Apps::itemDragged(int fromIndex, int toIndex)
{
//    qDebug() << "Moving from to:" << fromIndex << toIndex;
//    qDebug() << "WAS-------------------------";
//    for (int i = 0; i < appsList.size(); i++)
//        qDebug() << appsList[i].caption;

    int userAppIndex = -1;
    for (int i = 0; i < userAppsOrder.size(); i++)
    {
        if (userAppsOrder[i].first == appsList[fromIndex].caption)
            userAppIndex = i;
    }

    /*if (dndDest < dndSrc)
    {
        for (var i = dndDest + 1; i <= dndSrc; i++)
            model.get(i).id++
    }
    else
    {
        for (var i = dndSrc; i < dndDest; i++)
            model.get(i).id--
    }*/



    if (userAppIndex != -1)
    {
        if (toIndex < fromIndex)
        {
            for (int i = 0; i < userAppsOrder.size(); i++)
            {
                //qDebug() << i << ": " << userAppsOrder[i].second << " index is examining for (" << toIndex << ";" << fromIndex;
                if (userAppsOrder[i].second >= toIndex && userAppsOrder[i].second < fromIndex)
                    userAppsOrder[i].second++;
            }
        }
        else
            for (int i = 0; i < userAppsOrder.size(); i++)
            {
                if (userAppsOrder[i].second > fromIndex && userAppsOrder[i].second <= toIndex)
                    userAppsOrder[i].second--;
            }
    }
    if (userAppIndex == -1)
        userAppsOrder.append(qMakePair(appsList[fromIndex].caption, toIndex));
    else
        userAppsOrder[userAppIndex].second = toIndex;

    appsList.move(fromIndex, toIndex);

    saveData();

//    qDebug() << "NOW-------------------------";
//    for (int i = 0; i < appsList.size(); i++)
//        qDebug() << appsList[i].caption;
    //    qDebug() << "END-------------------------";
}

void DataSource_Apps::updateItems()
{
    prevCurrentGroup = currentGroup;
    appsList = GetFlatList(currentGroup);

    qSort(appsList);

    for (int i = 0; i < userAppsOrder.size(); i++)
    {
        for (int j = 0; j < appsList.size(); j++)
        {
            if (userAppsOrder[i].first == appsList[j].caption)
                appsList.move(j, qBound(0, userAppsOrder[i].second, appsList.size() - 1));
        }
    }
}

void DataSource_Apps::itemClicked(int newIndex)
{
    if(currentGroup == "" && newIndex == -1)
        return;

    if(newIndex != -1) {
        AppItem clickedItem = appsList[newIndex];
        if(clickedItem.relPath.isEmpty()) {
            recentApps->addRecentApp(clickedItem.desktopEntry);
            emit runDesktopFile(clickedItem.desktopEntry);
            return;
        }
    }

    if(newIndex == -1)
        currentGroup = "";
    else
        currentGroup = appsList[newIndex].relPath;

    if(prevCurrentGroup != currentGroup)
        updateItems();

    emit resetContent();
}
