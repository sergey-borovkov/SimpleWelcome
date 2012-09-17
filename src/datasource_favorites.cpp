#include "datasource_favorites.h"
#include <KFilePlacesModel>

DataSource_Favorites::DataSource_Favorites(QObject *parent)
    : DataSource(parent)
{
    KFilePlacesModel *places = new KFilePlacesModel();

    for(int i = 0; i < places->rowCount(); i++) {
        KBookmark bm = places->bookmarkForIndex(places->index(i, 0));
        AppItem newItem;
        newItem.caption = bm.fullText();
        newItem.desktopEntry = places->index(i, 0).data(KFilePlacesModel::UrlRole).toString();
        newItem.icon = bm.icon();

        favoritesList.append(newItem);
    }
}

int DataSource_Favorites::getItemCount()
{
    return favoritesList.count();
}


void DataSource_Favorites::getContent()
{
    for(int i = 0; i < favoritesList.count(); i++) {
        emit newItemData(QString("image://generalicon/appicon/%1").arg(favoritesList[i].icon), favoritesList[i].caption, i);
    }
}

void DataSource_Favorites::itemClicked(int newIndex)
{
    if (newIndex != -1)
        emit runDesktopFile(favoritesList[newIndex].desktopEntry);
}
