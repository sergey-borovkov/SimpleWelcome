#include "datasource_recentapps.h"
#include <kservicegroup.h>
#include <KConfigGroup>
#include <KDesktopFile>

DataSource_RecentApps::DataSource_RecentApps(QObject *parent)
    : DataSource(parent)
{
    KConfigGroup configGroup(KGlobal::config(), "General");
    QStringList recentAppsStringList = configGroup.readEntry("Recent applications", QStringList());

    foreach (QString recentApp, recentAppsStringList)
        addRecentApp(recentApp);
}

DataSource_RecentApps::~DataSource_RecentApps()
{
    QStringList desktopFiles;
    for (int i = 0; i < recentAppsList.size(); i++)
        desktopFiles.prepend(recentAppsList[i].desktopEntry);

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("Recent applications", desktopFiles);
    configGroup.sync();
}

void DataSource_RecentApps::addRecentApp(QString desktopFilePath)
{
    if(!KDesktopFile::isDesktopFile(desktopFilePath))
        return;

    bool isFound = false;

    for (int i = 0; i < recentAppsList.size(); i++)
    {
        if (recentAppsList[i].desktopEntry == desktopFilePath)
        {
            recentAppsList.move(i, 0);
            isFound = true;
            break;
        }
    }

    if (!isFound)
    {
        KDesktopFile desktopFile(desktopFilePath);
        AppItem newItem;

        newItem.caption = desktopFile.readName();
        newItem.icon = desktopFile.readIcon();
        newItem.desktopEntry = desktopFilePath;
        if (!newItem.caption.isEmpty())
            recentAppsList.prepend(newItem);
    }

    if (recentAppsList.size() > 7)
        recentAppsList.removeAt(7);

    emit resetContent();
}

#include <QMessageBox>

void DataSource_RecentApps::itemClicked(int newIndex)
{
    if (newIndex != -1)
    {
        QMessageBox::information(0, recentAppsList[newIndex].desktopEntry, recentAppsList[newIndex].caption);
        recentAppsList.move(newIndex, 0);
    }
}

void DataSource_RecentApps::getContent()
{
    for (int i = 0; i < recentAppsList.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(recentAppsList[i].icon), recentAppsList[i].caption, i);
}
