
#include "datasource_documents.h"

#include "sizescalculator.h"

#include <KDE/KRecentDocument>
#include <KDE/KDesktopFile>
#include <KDE/KFilePlacesModel>
#include <KDE/KIO/PreviewJob>
#include <KDE/KDirWatch>

#include <QtCore/QFile>
#include <QtCore/QTimerEvent>
#include <QtCore/QDir>
#include <QtGui/QPainter>


namespace
{
static const int _UPDATE_DELAY = 1000; // msec
static const int _MAX_RECENT_DOC_NUMBER = 7;
} //namespace



DataSource_Documents::DataSource_Documents(QObject *parent, SizesCalculator *inConstants)
    : DataSource(parent),
      m_previewJobPlugins(KIO::PreviewJob::availablePlugins()),
      constants(inConstants),
      runningJob(NULL),
      m_recentsChanged(false)
{
    KDirWatch *recents_watch = new KDirWatch(this);
    recents_watch->addDir(KRecentDocument::recentDocumentDirectory(), KDirWatch::WatchFiles);
    connect(recents_watch, SIGNAL(created(QString)), SLOT(onRecentsChanged(QString)));
    connect(recents_watch, SIGNAL(deleted(QString)), SLOT(onRecentsChanged(QString)));
    connect(recents_watch, SIGNAL(dirty(QString)), SLOT(onRecentsChanged(QString)));

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
    QSet<QString> favorites;
    for (int i = 0; i < places->rowCount(); i++)
        favorites.insert(places->index(i, 0).data(KFilePlacesModel::UrlRole).toString());

    QStringList recentDocsList = KRecentDocument::recentDocuments();

    AppItemList newDocsList;
    for (int i = 0; i < recentDocsList.size() && newDocsList.count() < _MAX_RECENT_DOC_NUMBER; i++) {
        if (!QFile::exists(recentDocsList[i]) || !KDesktopFile::isDesktopFile(recentDocsList[i]))
            continue;

        KDesktopFile desktopFile(recentDocsList[i]);
        if (desktopFile.noDisplay())
            continue;

        QString url_value = desktopFile.readUrl();
        if (favorites.contains(url_value))
            continue;

        // skip directories
        KUrl url = KUrl(url_value);
        if (url.isLocalFile() && QDir(url.toLocalFile()).exists())
            continue;

        AppItem newItem;

        newItem["caption"] = desktopFile.readName();
        newItem["id"] = newDocsList.count();
        newItem["imagePath"] = QString("image://generalicon/appicon/%1").arg(desktopFile.readIcon());
        newItem["desktopEntry"] = desktopFile.fileName();
        newItem["destination"] = url.url();

        if (m_pixmaps.contains(newItem["destination"].toString()))
            newItem["imagePath"] = QString("image://generalicon/docicon/%1").arg(newItem["destination"].toString());

        // Filtering unneeded documents
        bool isAdding = true;
        if (newItem["caption"].toString().isEmpty())
            continue;
        for (int j = 0; j < newDocsList.size(); j++)
            if (newDocsList[j]["destination"].toString() == newItem["destination"].toString()) {
                isAdding = false;
                break;
            }

        if (isAdding)
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
    int iconSize = constants->thumbnailsSize();
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
}

void DataSource_Documents::previewFailed(const KFileItem &/*item*/)
{
    //qDebug() << "Preview failed" << item.url();
}

void DataSource_Documents::previewJobFinished(KJob */*job*/)
{
    runningJob = NULL;
}

void DataSource_Documents::onRecentsChanged(QString /*name*/)
{
    // start time to skip numerous events about changes (to descrease number of updates)
    m_updateDelayTimer.start(_UPDATE_DELAY, this);
}

void DataSource_Documents::onUpdateAllowedChanged()
{
    if (m_recentsChanged && isUpdateAllowed()) {
        updateContent();
        m_recentsChanged = false;
    }
}

void DataSource_Documents::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_updateDelayTimer.timerId()) {
        m_updateDelayTimer.stop();
        if (isUpdateAllowed())
            updateContent();
        m_recentsChanged = !isUpdateAllowed();
        return;
    }
    DataSource::timerEvent(event);
}

void DataSource_Documents::createDocumentsPreviews(KFileItemList list)
{
    if (runningJob)
        runningJob->kill();

    KIO::PreviewJob *job = KIO::filePreview(list, QSize(constants->thumbnailsSize(), constants->thumbnailsSize()), &m_previewJobPlugins);
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);
    runningJob = job;

    connect(job, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)), SLOT(resultPreviewJob(KFileItem, QPixmap)));
    connect(job, SIGNAL(failed(const KFileItem &)), SLOT(previewFailed(const KFileItem &)));
    connect(job, SIGNAL(result(KJob*)), SLOT(previewJobFinished(KJob*)));
}

void DataSource_Documents::itemClicked(int newIndex)
{
    if (newIndex != -1) {
        docsList.move(newIndex, 0);
        emit runDesktopFile(docsList[0]["desktopEntry"].toString());
    }
}
