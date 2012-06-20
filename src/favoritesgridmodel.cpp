#include "favoritesgridmodel.h"
#include <KFilePlacesModel>

FavoritesGridModel::FavoritesGridModel(QObject *parent)
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

QList<AppItem> FavoritesGridModel::GetList(QString currentGroup) const
{
    static QList<AppItem> out;
    if (!out.isEmpty())
        return out;

    KFilePlacesModel *places = new KFilePlacesModel();

    for(int i = 0; i < places->rowCount(); i++)
    {
        KBookmark bm = places->bookmarkForIndex(places->index(i, 0));
        AppItem newItem;
        newItem.caption = bm.fullText(); //places->index(i, 0).data(Qt::DisplayRole).toString();
        newItem.desktopEntry = places->index(i, 0).data(KFilePlacesModel::UrlRole).toString();
        newItem.icon = bm.icon(); // places->index(i, 0).data(Qt::DecorationRole).toString();

        out.append(newItem);
   }

   return out;
}

int FavoritesGridModel::rowCount( const QModelIndex & /*parent*/ ) const
{
    return GetList(currentGroup).size();
}

QVariant FavoritesGridModel::data( const QModelIndex &index, int role ) const
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

void FavoritesGridModel::itemClicked(int newIndex)
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
