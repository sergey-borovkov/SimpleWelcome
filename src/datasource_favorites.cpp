#include "datasource_favorites.h"
#include <KFilePlacesModel>

DataSource_Favorites::DataSource_Favorites(QObject *parent)
    : DataSource(parent)
{
    KFilePlacesModel *places = new KFilePlacesModel();

    QList<AppItem> list;
    for (int i = 0; i < places->rowCount(); i++) {
        KBookmark bm = places->bookmarkForIndex(places->index(i, 0));
        AppItem newItem;

        newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(bm.icon());
        newItem["caption"] = bm.fullText();
        newItem["id"] = list.count();
        newItem["desktopEntry"] = places->index(i, 0).data(KFilePlacesModel::UrlRole);

        list.append(newItem);
    }

    QSet<AppItem> set = QSet<AppItem>::fromList(list);

    for (int i = 0; i < list.size() && favoritesList.size() < 14; i++)  // Limiting favorites item count to two rows (14 items)
        if (set.contains(list[i])) {
            set.remove(list[i]);
            favoritesList.append(list[i]);
        }
}

int DataSource_Favorites::getItemCount()
{
    return favoritesList.count();
}

QString DataSource_Favorites::itemUrlDnd(int id)
{
    if (id >= 0 && id < favoritesList.count())
        return favoritesList[id]["desktopEntry"].toString();
    return QString();
}


QVariantMap DataSource_Favorites::getContent(int index)
{
    return favoritesList[index];
}

void DataSource_Favorites::itemClicked(int newIndex)
{
    if (newIndex != -1)
        emit runDesktopFile(favoritesList[newIndex]["desktopEntry"].toString());
}
