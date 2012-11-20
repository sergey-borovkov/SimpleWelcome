#include "datasource_recentapps.h"
#include <kservicegroup.h>
#include <KConfigGroup>
#include <KDesktopFile>

DataSource_RecentApps::DataSource_RecentApps(QObject *parent)
    : DataSource(parent)
{
    KConfigGroup configGroup(KGlobal::config(), "General");
    QStringList recentAppsStringList = configGroup.readEntry("Recent applications", QStringList());

    foreach(QString recentApp, recentAppsStringList)
    addRecentApp(recentApp);
}

int DataSource_RecentApps::getItemCount()
{
    return recentAppsList.count();
}

QString DataSource_RecentApps::itemUrlDnd(int id)
{
    if (id >= 0 && id < recentAppsList.count())
        return QString::fromAscii("file://") + recentAppsList[id]["desktopEntry"].toString();
    return QString();
}

void DataSource_RecentApps::saveData()
{
    QStringList desktopFiles;
    for (int i = 0; i < recentAppsList.size(); i++)
        desktopFiles.prepend(recentAppsList[i]["desktopEntry"].toString());

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("Recent applications", desktopFiles);
    configGroup.sync();
}

void DataSource_RecentApps::addRecentApp(QString desktopFilePath)
{
    if (!KDesktopFile::isDesktopFile(desktopFilePath))
        return;

    bool isFound = false;

    for (int i = 0; i < recentAppsList.size(); i++) {
        if (recentAppsList[i]["desktopEntry"].toString() == desktopFilePath) {
            recentAppsList.move(i, 0);
            isFound = true;
            break;
        }
    }

    if (!isFound) {
        KDesktopFile desktopFile(desktopFilePath);
        AppItem newItem;

        newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(desktopFile.readIcon());
        newItem["caption"] = desktopFile.readName();
        newItem["pinned"] = /*i < 4 ? true :*/ false;
        newItem["desktopEntry"] = desktopFilePath;

        if (!newItem["caption"].toString().isEmpty())
            recentAppsList.prepend(newItem);
    }

    if (recentAppsList.size() > 7)
        recentAppsList.removeAt(7);

    for (int i = 0; i < recentAppsList.size(); i++)
        recentAppsList[i]["id"] = i;


    saveData();
    emit resetContent();
}

void DataSource_RecentApps::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        recentAppsList.move(newIndex, 0);
        saveData();
        emit runDesktopFile(recentAppsList[0]["desktopEntry"].toString());
    }
}

void DataSource_RecentApps::itemPinnedToggle(int index)
{
    bool pinned = !recentAppsList[index]["pinned"].toBool();
    recentAppsList[index]["pinned"] = pinned;
    emit updateItemData(index, "pinned", pinned);
}

QVariantMap DataSource_RecentApps::getContent(int index)
{
    return recentAppsList[index];
}

void DataSource_RecentApps::itemDragged(int fromIndex, int toIndex)
{
    recentAppsList.move(fromIndex, toIndex);

    for (int i = 0; i < recentAppsList.size(); i++)
        recentAppsList[i]["id"] = i;

    saveData();
}
