#include "documentsgridmodel.h"
#include <KRecentDocument>
#include <KDesktopFile>
#include <QStringList>
#include <QFile>

DocumentsGridModel::DocumentsGridModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names;
    names[ CaptionRole ] = "caption";
    names[ ImagePathRole ] = "imagePath";
    setRoleNames(names);
}

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;
};

QList<AppItem> DocumentsGridModel::GetList(QString currentGroup) const
{
    static QList<AppItem> out;
    if (!out.isEmpty())
        return out;

    QStringList recentDocsList = KRecentDocument::recentDocuments();
    foreach (QString fileName, recentDocsList)
    {
        if(! KDesktopFile::isDesktopFile(fileName))
            continue;
        if(! QFile::exists(fileName))
            continue;

        KDesktopFile desktopFile(fileName);
        if(desktopFile.noDisplay())
            continue;

        AppItem newItem;
        newItem.caption = desktopFile.readName();
        newItem.icon = desktopFile.readIcon();
        newItem.desktopEntry = desktopFile.fileName();
        if (!newItem.caption.isEmpty())
            out.append(newItem);
    }
    return out;
}

int DocumentsGridModel::rowCount( const QModelIndex & /*parent*/ ) const
{
    return GetList(currentGroup).size();
}

QVariant DocumentsGridModel::data( const QModelIndex &index, int role ) const
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

void DocumentsGridModel::itemClicked(int newIndex)
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
