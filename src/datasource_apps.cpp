
#include "datasource_apps.h"
#include "datasource_recentapps.h"

#include <KDE/KServiceGroup>
#include <KDE/KConfigGroup>
#include <KDE/KSycoca>


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
            newItem["desktopEntry"] = service->entryPath();
            newItem["group"] = group;

            out.append(newItem);
        } else if (p->isType(KST_KServiceGroup)) {
            const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

            if (serviceGroup->noDisplay() || serviceGroup->childCount() == 0)
                continue;

            AppItemList temp = GetFlatList(serviceGroup->relPath());
            for (int i = 0; i < temp.size(); i++) {
                bool isFound = false;
                for (int j = 0; j < out.size(); j++)
                    if (out[j]["desktopEntry"].toString() == temp[i]["desktopEntry"].toString()) {
                        isFound = true;
                        break;
                    }
                if (!isFound)
                    out.append(temp[i]);
            }
        } else {
            qDebug() << "KServiceGroup: Unexpected object in list!";
            continue;
        }
    }
    return out;
}

DataSource_Apps::DataSource_Apps(QObject *parent, DataSource_RecentApps *inRecentApps)
    : DataSource(parent), prevCurrentGroup("-1"), recentApps(inRecentApps), m_isDbChanged(false)
{
    updateItems(false);
    connect(KSycoca::self(), SIGNAL(databaseChanged(QStringList)), this, SLOT(ksycocaChanged(QStringList)));
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

void DataSource_Apps::onUpdateAllowedChanged()
{
    if (isUpdateAllowed() && m_isDbChanged) {
        m_isDbChanged = false;
        updateItems(true);
    }
}

QVariantMap DataSource_Apps::getContent(int index)
{
    return appsList[index];
}

bool appItemLessThan(const AppItem &a, const QVariantMap &b)
{
    return a.value("caption").toString().compare(b.value("caption").toString(), Qt::CaseInsensitive) < 0;
}

void DataSource_Apps::updateItems(bool isResetContent/* = true*/)
{
    prevCurrentGroup = currentGroup;

    AppItemList newList = GetFlatList(currentGroup);
    qSort(newList.begin(), newList.end(), appItemLessThan);
    for (int i = 0; i < newList.count(); i++)
        newList[i]["id"] = i;

    if (newList != appsList) {
        appsList = newList;

        if (isResetContent)
            emit resetContent();
    }
}

void DataSource_Apps::ksycocaChanged(const QStringList &changes)
{
    if (changes.contains("apps")) {
        if (isUpdateAllowed())
            updateItems(true);  // if allowed => update now
        m_isDbChanged = !isUpdateAllowed();  // delay update until it is allowed
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
