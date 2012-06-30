#include "recentappsgridmodel.h"
#include <kservicegroup.h>
#include <kdebug.h>
#include <KConfigGroup>
#include <KDesktopFile>

RecentAppsGridModel::RecentAppsGridModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names;
    names[ CaptionRole ] = "caption";
    names[ ImagePathRole ] = "imagePath";
    setRoleNames(names);


    KConfigGroup configGroup(KGlobal::config(), "General");
    QStringList recentAppsList = configGroup.readEntry("Recent applications", QStringList());

    foreach (QString recentApp, recentAppsList)
    {
        if(!KDesktopFile::isDesktopFile(recentApp))
            continue;

        KDesktopFile desktopFile(recentApp);
        AppItem newItem;

        newItem.caption = desktopFile.readName();
        newItem.icon = desktopFile.readIcon();
        newItem.desktopEntry = recentApp;
        if (!newItem.caption.isEmpty())
            m_RecentAppsList.append(newItem);
    }
}

RecentAppsGridModel::~RecentAppsGridModel()
{
    QStringList desktopFiles;
    for (int i = 0; i < m_RecentAppsList.size(); i++)
        desktopFiles.append(m_RecentAppsList[i].desktopEntry);

    KConfigGroup configGroup(KGlobal::config(), "General");
    configGroup.writeEntry("Recent applications", desktopFiles);
    configGroup.sync();
}

QList<AppItem> RecentAppsGridModel::GetList(QString currentGroup) const
{

    return m_RecentAppsList;
}

int RecentAppsGridModel::rowCount( const QModelIndex & /*parent*/ ) const
{
    return GetList(currentGroup).size();
}

QVariant RecentAppsGridModel::data( const QModelIndex &index, int role ) const
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

void RecentAppsGridModel::itemClicked(int newIndex)
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
