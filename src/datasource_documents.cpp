#include "datasource_documents.h"
#include <KRecentDocument>
#include <KDesktopFile>
#include <QStringList>
#include <QFile>

void DataSource_Documents::updateContent()
{
    if (!docsList.isEmpty())
        return;

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
            docsList.append(newItem);

        emit newItemData(QString("image://generalicon/appicon/%1").arg(newItem.icon), newItem.caption);
    }
}

#include <QMessageBox>

void DataSource_Documents::itemClicked(int newIndex)
{
    if (newIndex != -1)
        QMessageBox::information(0, docsList[newIndex].desktopEntry, docsList[newIndex].caption);
}
