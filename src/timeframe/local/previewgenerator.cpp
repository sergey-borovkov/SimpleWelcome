/* Timeframe
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 3,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "localcontentmodel.h"
#include "previewgenerator.h"

#include <QtCore/QStringList>
#include <QtGui/QPainter>

#include <KFile>
#include <KIcon>

PreviewGenerator *previewGenerator(const QString &type)
{
    static PreviewGenerator *galleryInstance = 0;
    static PreviewGenerator *timeLineInstance = 0;

    if (type == QLatin1String("gallery")) {
        if (!galleryInstance)
            galleryInstance = new PreviewGenerator;
        return galleryInstance;
    }
    else {
        if (!timeLineInstance)
            timeLineInstance = new PreviewGenerator;
        return timeLineInstance;
    }
}

PreviewGenerator::PreviewGenerator()
{
    defaultPreview.load(":/pla-empty-box.png");
    videoPixmap.load(":/play-empty.png");
    m_plugins = KIO::PreviewJob::availablePlugins();
}

void PreviewGenerator::notifyModelAboutPreview(const QString &url)
{
    PreviewItemIterator it = m_searchedItems.find(url);
    if(it != m_searchedItems.end()) {
        it.value().model->previewReady(url);
        m_searchedItems.erase(it);
    }
}

void PreviewGenerator::previewJobResult(const KFileItem &item, const QPixmap &pixmap)
{
    QPixmap pict = pixmap;
    if (item.mimetype().startsWith("video/")) {
        QPainter p(&pict);
        QPixmap scaledPixmap = videoPixmap.scaled(pict.width() / 2, pict.height() / 2,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawPixmap(pict.width() / 2 - scaledPixmap.width() / 2, pict.height() / 2 - scaledPixmap.height() / 2 ,  scaledPixmap);
    }

    m_previews.insert(item.localPath(), pict);
    notifyModelAboutPreview(item.localPath());
}

void PreviewGenerator::previewJobFailed(const KFileItem &item)
{
    if(m_searchedItems.find(item.localPath()) == m_searchedItems.end())
        return;

    KIcon icon(item.iconName(), 0, item.overlays());
    QPixmap pixmap = icon.pixmap(500);
    m_previews.insert(item.localPath(), pixmap);
    notifyModelAboutPreview(item.localPath());
}

QPixmap PreviewGenerator::takePreviewPixmap(QString filePath)
{
    QHash<QString, QPixmap>::iterator it;
    it = m_previews.find(filePath);
    if(it != m_previews.end()) {
        QPixmap pixmap = it.value();
        m_previews.erase(it);
        return pixmap;
    }
    else {
        return defaultPreview;
    }
}

void PreviewGenerator::itemShown(QString path, QObject *dayModel)
{
    PreviewItemIterator it = m_searchedItems.find(path);
    // duplicate request
    if(it != m_searchedItems.end()) {
        return;
    }

    LocalDayFilterModel *filteredModel = static_cast<LocalDayFilterModel *>(dayModel);
    KFileItemList fileList;
    KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(path), true);
    fileList.append(fileItem);

    KIO::PreviewJob *job = KIO::filePreview(fileList, QSize(512, 512), &m_plugins);
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);

    PreviewItem item;
    item.model = filteredModel;
    item.job = job;
    m_searchedItems.insert(path, item);

    connect(job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(previewJobResult(const KFileItem&, const QPixmap&)));
    connect(job, SIGNAL(failed(const KFileItem&)), SLOT(previewJobFailed(const KFileItem &)));
}

void PreviewGenerator::itemHidden(QString path, QObject *dayModel)
{
    LocalDayFilterModel *filteredModel = static_cast<LocalDayFilterModel *>(dayModel);
    Q_UNUSED(filteredModel)
    PreviewItemIterator it = m_searchedItems.find(path);
    if(it != m_searchedItems.end()) {
        it.value().job->kill();
        m_searchedItems.erase(it);
    }
    m_previews.remove(path);
}
