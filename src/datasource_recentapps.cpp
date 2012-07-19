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
    {
        if(!KDesktopFile::isDesktopFile(recentApp))
            continue;

        KDesktopFile desktopFile(recentApp);
        AppItem newItem;

        newItem.caption = desktopFile.readName();
        newItem.icon = desktopFile.readIcon();
        newItem.desktopEntry = recentApp;
        if (!newItem.caption.isEmpty())
            recentAppsList.append(newItem);
    }
}

DataSource_RecentApps::~DataSource_RecentApps()
{
    QStringList desktopFiles;
    for (int i = 0; i < recentAppsList.size(); i++)
        desktopFiles.append(recentAppsList[i].desktopEntry);

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("Recent applications", desktopFiles);
    configGroup.sync();
}

#include <QMessageBox>

void DataSource_RecentApps::itemClicked(int newIndex)
{
    if (newIndex != -1)
        QMessageBox::information(0, recentAppsList[newIndex].desktopEntry, recentAppsList[newIndex].caption);
}

void DataSource_RecentApps::updateContent()
{
    for (int i = 0; i < recentAppsList.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(recentAppsList[i].icon), recentAppsList[i].caption);
}
