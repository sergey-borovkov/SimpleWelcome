#include "datasource_recentapps.h"
#include <kservicegroup.h>
#include <KConfigGroup>
#include <KDesktopFile>
#include <KDE/KSycoca>
#include <QFile>

DataSource_RecentApps::DataSource_RecentApps(QObject *parent)
    : DataSource(parent), m_isDbChanged(true)
{
    KConfigGroup configGroup(KGlobal::config(), "General");
    QStringList recentAppsStringList = configGroup.readEntry("Recent applications", QStringList());
    QStringList pinnedRecentApps = configGroup.readEntry("Pinned recent applications", QStringList());

    foreach(QString recentApp, recentAppsStringList) {
        addRecentApp(recentApp, pinnedRecentApps.contains(recentApp), true);
    }

    connect(KSycoca::self(), SIGNAL(databaseChanged(QStringList)), SLOT(ksycocaChanged(QStringList)));
}

void DataSource_RecentApps::ksycocaChanged(const QStringList changes)
{
    if (changes.contains("apps")) {

        m_isDbChanged = true;
    }
}

bool DataSource_RecentApps::checkApps()
{
    bool isChanged = false;
    if (m_isDbChanged) {
        m_isDbChanged = false;

        for (int i = 0; i < recentAppsList.size(); i++)
            if (!QFile::exists(recentAppsList[i]["desktopEntry"].toString())) {
                recentAppsList.removeAt(i);
                i--;
                isChanged = true;
            }

        if (isChanged)
            updateContent();
    }
    return isChanged;
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
    QStringList pinnedApps;
    for (int i = 0; i < recentAppsList.size(); i++) {
        desktopFiles.prepend(recentAppsList[i]["desktopEntry"].toString());
        if (recentAppsList[i]["pinned"].toBool())
            pinnedApps.append(recentAppsList[i]["desktopEntry"].toString());
    }

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("Recent applications", desktopFiles);
    configGroup.writeEntry("Pinned recent applications", pinnedApps);
    configGroup.sync();
}

void DataSource_RecentApps::moveIconSkippingPinned(int id)
{
    if (!recentAppsList[id]["pinned"].toBool()) {
        for (int i = 0; i < id; i++)
            if (!recentAppsList[i]["pinned"].toBool())
                qSwap(recentAppsList[id], recentAppsList[i]);
    }
}

void DataSource_RecentApps::addRecentApp(QString desktopFilePath, bool isPinned, bool isSimplyLoad)
{
    if (!KDesktopFile::isDesktopFile(desktopFilePath))
        return;

    bool isFound = false;

    for (int i = 0; i < recentAppsList.size(); i++) {
        if (recentAppsList[i]["desktopEntry"].toString() == desktopFilePath) {
            if (isSimplyLoad)
                recentAppsList.move(i, 0);
            else
                moveIconSkippingPinned(i);
            isFound = true;
            break;
        }
    }
    if (!isFound) {
        KDesktopFile desktopFile(desktopFilePath);
        AppItem newItem;

        newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(desktopFile.readIcon());
        newItem["caption"] = desktopFile.readName();
        newItem["pinned"] = isPinned;
        newItem["desktopEntry"] = desktopFilePath;
        QString description = desktopFile.readComment();
        if(description.isEmpty())
            description = desktopFile.readGenericName();
        newItem["description"] = description;

        if (!newItem["caption"].toString().isEmpty()) {
            if (isSimplyLoad)
                recentAppsList.prepend(newItem);
            else {
                recentAppsList.append(newItem);
                moveIconSkippingPinned(recentAppsList.size() - 1);
            }
        }
    }

    if (recentAppsList.size() > 7)
        recentAppsList.removeAt(7);

    updateContent();
}

void DataSource_RecentApps::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        emit runDesktopFile(recentAppsList[newIndex]["desktopEntry"].toString());
        moveIconSkippingPinned(newIndex);
        saveData();
    }
}

void DataSource_RecentApps::updateContent()
{
    if (m_isDbChanged)
        if (checkApps())
            return;

    for (int i = 0; i < recentAppsList.size(); i++)
        recentAppsList[i]["id"] = i;

    saveData();
    emit resetContent();
}

void DataSource_RecentApps::itemPinnedToggle(int index)
{
    bool pinned = !recentAppsList[index]["pinned"].toBool();
    recentAppsList[index]["pinned"] = pinned;
    saveData();
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
