#include "datasource_documents.h"
#include <KRecentDocument>
#include <KDE/KDesktopFile>
#include <QStringList>
#include <QFile>
#include <KFilePlacesModel>

DataSource_Documents::DataSource_Documents(QObject* parent)
    : DataSource(parent)
{
    // Fix someday, ugly solution
    KFilePlacesModel *places = new KFilePlacesModel();
    QStringList favoritesList;
    for (int i = 0; i < places->rowCount(); i++)
        favoritesList.append(places->index(i, 0).data(KFilePlacesModel::UrlRole).toString());

    QStringList recentDocsList = KRecentDocument::recentDocuments();
    for (int i = 0; i < recentDocsList.size() && docsList.count() < 7; i++) {
        if (!QFile::exists(recentDocsList[i]) || !KDesktopFile::isDesktopFile(recentDocsList[i]))
            continue;

        KDesktopFile desktopFile(recentDocsList[i]);
        if (desktopFile.noDisplay())
            continue;

        if (favoritesList.contains(desktopFile.readUrl()))
            continue;

        AppItem newItem;
        newItem.caption = desktopFile.readName();
        newItem.icon = desktopFile.readIcon();
        newItem.desktopEntry = desktopFile.fileName();
        if (!newItem.caption.isEmpty())
            docsList.append(newItem);
    }
}

int DataSource_Documents::getItemCount()
{
    return docsList.count();
}

QString DataSource_Documents::itemUrlDnd(int id)
{
    if (id >= 0 && id < docsList.count()) {
        KDesktopFile file(docsList[id].desktopEntry);
        return file.readUrl();
    }
    return QString();
}

void DataSource_Documents::getContent()
{
    for (int i = 0; i < docsList.size(); i++)
        emit newItemData(QString("image://generalicon/appicon/%1").arg(docsList[i].icon), docsList[i].caption, i);
}

void DataSource_Documents::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        docsList.move(newIndex, 0);
        emit runDesktopFile(docsList[0].desktopEntry);
    }
}
