#include "datasource_favorites.h"
#include <KFilePlacesModel>

void DataSource_Favorites::updateContent()
{
    if (!favoritesList.isEmpty())
        return;

    KFilePlacesModel *places = new KFilePlacesModel();

    for(int i = 0; i < places->rowCount(); i++)
    {
        KBookmark bm = places->bookmarkForIndex(places->index(i, 0));
        AppItem newItem;
        newItem.caption = bm.fullText();
        newItem.desktopEntry = places->index(i, 0).data(KFilePlacesModel::UrlRole).toString();
        newItem.icon = bm.icon();

        favoritesList.append(newItem);
        emit newItemData(QString("image://generalicon/appicon/%1").arg(newItem.icon), newItem.caption);
   }
}

#include <QMessageBox>

void DataSource_Favorites::itemClicked(int newIndex)
{
    if (newIndex != -1)
        QMessageBox::information(0, favoritesList[newIndex].desktopEntry, favoritesList[newIndex].caption);
}
