#include "datasource_favorites.h"
#include <KDE/KFilePlacesModel>

#include <QtCore/QTimerEvent>


namespace
{
static const int _UPDATE_DELAY = 1000; // msec
static const int _MAX_ITEMS_NUMBER = 14; // maximum number of items
}


DataSource_Favorites::DataSource_Favorites(QObject *parent)
    : DataSource(parent)
    , m_placesModel(new KFilePlacesModel(this))
{
    reloadItems();

    connect(m_placesModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(modelReset()), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), SLOT(placesChanged()));
    connect(m_placesModel, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(placesChanged()));
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

void DataSource_Favorites::placesChanged()
{
    m_timer.start(_UPDATE_DELAY, this);
}

void DataSource_Favorites::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timer.timerId()) {
        m_timer.stop();
        reloadItems();
        return;
    }
    DataSource::timerEvent(event);
}

void DataSource_Favorites::reloadItems()
{
    QList<AppItem> new_list;
    for (int i = 0; i < m_placesModel->rowCount() && new_list.size() < _MAX_ITEMS_NUMBER; i++) {
        QModelIndex index = m_placesModel->index(i, 0);
        KBookmark bm = m_placesModel->bookmarkForIndex(index);
        if (!bm.isNull() && !m_placesModel->isHidden(index)) {
            AppItem newItem;
            newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(bm.icon());
            newItem["caption"] = bm.fullText();
            newItem["id"] = new_list.size();
            newItem["desktopEntry"] = index.data(KFilePlacesModel::UrlRole);
            new_list.append(newItem);
        }
    }

    favoritesList = new_list;
    emit resetContent();
}
