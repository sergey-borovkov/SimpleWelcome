
#include "datasource_documents.h"

#include "sizescalculator.h"

#include <KDE/KRecentDocument>
#include <KDE/KDesktopFile>
#include <KDE/KFilePlacesModel>
#include <KDE/KIO/PreviewJob>

#include <QtCore/QFile>
#include <QtGui/QPainter>



DataSource_Documents::DataSource_Documents(QObject *parent, SizesCalculator *inConstants)
    : DataSource(parent),
      m_previewJobPlugins(KIO::PreviewJob::availablePlugins()),
      constants(inConstants)
{
    updateContent();
}

int DataSource_Documents::getItemCount() const
{
    return docsList.count();
}

QString DataSource_Documents::itemUrlDnd(int id) const
{
    if (id >= 0 && id < docsList.count()) {
        KDesktopFile file(docsList[id]["desktopEntry"].toString());
        return file.readUrl();
    }
    return QString();
}

QPixmap DataSource_Documents::getPreview(QString destination) const
{
    destination = KUrl(destination).url();
    if (m_pixmaps.contains(destination))
        return  m_pixmaps[destination];
    else {
        qDebug() << destination;
        qDebug() << "IMAGE NOT FOUND! A BUG";
        qDebug() << m_pixmaps.keys();
    }
    return QPixmap();
}

QVariantMap DataSource_Documents::getContent(int index) const
{
    return docsList[index];
}

void DataSource_Documents::updateContent()
{
    // Fix someday, ugly solution
    KFilePlacesModel *places = new KFilePlacesModel();
    QStringList favoritesList;
    for (int i = 0; i < places->rowCount(); i++)
        favoritesList.append(places->index(i, 0).data(KFilePlacesModel::UrlRole).toString());

    QStringList recentDocsList = KRecentDocument::recentDocuments();

    AppItemList newDocsList;
    for (int i = 0; i < recentDocsList.size() && newDocsList.count() < 7; i++) {
        if (!QFile::exists(recentDocsList[i]) || !KDesktopFile::isDesktopFile(recentDocsList[i]))
            continue;

        KDesktopFile desktopFile(recentDocsList[i]);
        if (desktopFile.noDisplay())
            continue;

        if (favoritesList.contains(desktopFile.readUrl()))
            continue;

        AppItem newItem;

        newItem["caption"] = desktopFile.readName();
        newItem["id"] = newDocsList.count();
        newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(desktopFile.readIcon());
        newItem["desktopEntry"] = desktopFile.fileName();
        newItem["destination"] = KUrl(desktopFile.readUrl()).url();

        if (m_pixmaps.contains(newItem["destination"].toString()))
            newItem["imagePath"] = QString("image://generalicon/docicon/%1").arg(newItem["destination"].toString());

        if (!newItem["caption"].toString().isEmpty())
            newDocsList.append(newItem);
    }

    if (newDocsList != docsList) {
        docsList = newDocsList;
        updateThumbnails();
        emit resetContent();
    }
}

void DataSource_Documents::updateThumbnails()
{
    KFileItemList previewRequestList;

    for (int i = 0; i < docsList.size(); i++) {
        AppItem &newItem = docsList[i];

        bool thumbnailAvailable = m_pixmaps.contains(newItem["destination"].toString());
        if (thumbnailAvailable)
            newItem["imagePath"] = QString("image://generalicon/docicon/%1").arg(newItem["destination"].toString());

        if (!thumbnailAvailable) {
            KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, newItem["destination"].toString(), false);
            previewRequestList.append(fileItem);
        }
    }

    createDocumentsPreviews(previewRequestList);
}

void DataSource_Documents::iconSizeChanged()
{
    m_pixmaps.clear();
    for (int i = 0; i < docsList.size(); i++) {
        KDesktopFile desktopFile(docsList[i]["desktopEntry"].toString());
        QString newIcon = QString("image://generalicon/appicon/%1").arg(desktopFile.readIcon());
        if (newIcon != docsList[i]["imagePath"]) {
            docsList[i]["imagePath"] = newIcon;
            emit updateItemData(i, "imagePath", docsList[i]["imagePath"].toString());
        }
    }
    updateThumbnails();
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
    p.translate((iconSize - pixmap.width()) / 2, (iconSize - pixmap.height()) / 2);
    p.drawRoundedRect(0, 0, pixmap.width() - 1, pixmap.height() - 1, 7, 7, Qt::AbsoluteSize);

    m_pixmaps[item.url().url()] = pix;

    for (int i = 0; i < docsList.size(); i++) {
        if (docsList[i]["destination"].toString() == item.url().url()) {
            docsList[i]["imagePath"] = QString("image://generalicon/docicon/%1").arg(docsList[i]["destination"].toString());
            emit updateItemData(i, "imagePath", docsList[i]["imagePath"].toString());
        }
    }
    //emit resetContent();
}

void DataSource_Documents::previewFailed(const KFileItem &/*item*/)
{
    //qDebug() << "Preview failed" << item.url();
}

void DataSource_Documents::createDocumentsPreviews(KFileItemList list)
{
    KIO::PreviewJob *job = KIO::filePreview(list, QSize(constants->iconSize(), constants->iconSize()), &m_previewJobPlugins);
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);
    connect(job, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)), SLOT(resultPreviewJob(KFileItem, QPixmap)));
    connect(job, SIGNAL(failed(const KFileItem &)), SLOT(previewFailed(const KFileItem &)));
}

void DataSource_Documents::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        docsList.move(newIndex, 0);
        emit runDesktopFile(docsList[0]["desktopEntry"].toString());
    }
}
