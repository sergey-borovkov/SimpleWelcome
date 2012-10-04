#include "datasource_documents.h"
#include <KRecentDocument>
#include <KDE/KDesktopFile>
#include <QStringList>
#include <QFile>
#include <KFilePlacesModel>
#include <KIO/PreviewJob>
#include "swapp.h"
#include <QPainter>

DataSource_Documents::DataSource_Documents(QObject* parent, QMLConstants *inConstants)
    : DataSource(parent),
      m_previewJobPlugins(KIO::PreviewJob::availablePlugins()),
      constants(inConstants)
{
    updateContent();
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

QIcon DataSource_Documents::getIcon(QString destination)
{
    destination = KUrl(destination).url();
    if (m_pixmaps.contains(destination))
        return  m_pixmaps[destination];
    else {
        qDebug() << destination;
        qDebug() << "IMAGE NOT FOUND! A BUG";
        qDebug() << m_pixmaps.keys();
    }
    return QIcon();
}

void DataSource_Documents::getContent()
{
    for (int i = 0; i < docsList.size(); i++)
    {
        if (m_pixmaps.contains(docsList[i].destination))
            emit newItemData(QString("image://generalicon/docicon/%1").arg(docsList[i].destination), docsList[i].caption, i);
        else
            emit newItemData(QString("image://generalicon/appicon/%1").arg(docsList[i].icon), docsList[i].caption, i);
    }
}

void DataSource_Documents::updateContent()
{
    // Fix someday, ugly solution
    KFilePlacesModel *places = new KFilePlacesModel();
    QStringList favoritesList;
    for (int i = 0; i < places->rowCount(); i++)
        favoritesList.append(places->index(i, 0).data(KFilePlacesModel::UrlRole).toString());

    QStringList recentDocsList = KRecentDocument::recentDocuments();
    KFileItemList previewRequestList;

    QList<AppItem> newDocsList;
    for (int i = 0; i < recentDocsList.size() && newDocsList.count() < 7; i++) {
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
        newItem.destination = KUrl(desktopFile.readUrl()).url();

        if (!newItem.caption.isEmpty())
            newDocsList.append(newItem);

        if (!m_pixmaps.contains(newItem.destination)) {
            KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, newItem.destination, false);
            previewRequestList.append(fileItem);
        }
    }

    if (newDocsList != docsList) {
        docsList = newDocsList;
        createDocumentsPreviews(previewRequestList);
        emit resetContent();
        qDebug() << "Documents content update";
    }
}

void DataSource_Documents::resultPreviewJob(const KFileItem &item, const QPixmap &pixmap)
{
    int iconSize = constants->iconSize();
    //qDebug() << item.url().url();
    QPixmap pix(iconSize, iconSize);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    QBrush brush(pixmap);
    QPen pen;

    pen.setColor(Qt::transparent);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(0);

    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setBrush(brush);
    p.setPen(pen);
    p.translate((iconSize - pixmap.width())/2, (iconSize - pixmap.height())/2);
    p.drawRoundedRect(0, 0, pixmap.width(), pixmap.height(), 7, 7, Qt::AbsoluteSize);

    m_pixmaps[item.url().url()] = pix;
    for (int i = 0; i < docsList.size(); i++) {
        if (docsList[i].destination == item.url().url())
            emit updateItemData(i, "imagePath", QString("image://generalicon/docicon/%1").arg(docsList[i].destination));
    }
    //emit resetContent();
}

void DataSource_Documents::previewFailed(const KFileItem &/*item*/)
{
    //qDebug() << "Preview failed" << item.url();
}

void DataSource_Documents::createDocumentsPreviews(KFileItemList list)
{
    KIO::PreviewJob *job = KIO::filePreview( list, constants->iconSize(), 0, 0, 0, true, false, &m_previewJobPlugins );
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);
    connect(job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(resultPreviewJob(KFileItem,QPixmap)));
    connect(job, SIGNAL(failed(const KFileItem&)), SLOT(previewFailed(const KFileItem &)));
}

void DataSource_Documents::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        docsList.move(newIndex, 0);
        emit runDesktopFile(docsList[0].desktopEntry);
    }
}
