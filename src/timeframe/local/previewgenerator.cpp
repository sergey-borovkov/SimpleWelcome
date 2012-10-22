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
    : m_plugins(KIO::PreviewJob::availablePlugins())
    , m_model(0)
{
    videoPixmap.load(":/play-empty.png");
    m_defaultPreview.load(":/pla-empty-box.png");
}

void PreviewGenerator::previewComplete(PreviewGenerator::PreviewItemIterator it)
{
    if(it != m_pendingItems.end()) {
        PreviewItem &item = it.value();
        // remove from pending items if only one request left
        if(item.count == 1) {
            m_pendingItems.erase(it);
        } else {
            item.count--;
        }
    }
}

void PreviewGenerator::setModel(LocalContentModel *model)
{
    m_model = model;
}

void PreviewGenerator::notifyModelAboutPreview(const QString &url)
{
    if (m_model)
        m_model->previewReady(url);
}

void PreviewGenerator::previewJobResult(const KFileItem &item, const QPixmap &pixmap)
{
    previewComplete(m_pendingItems.find(item.localPath()));
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
    previewComplete(m_pendingItems.find(item.localPath()));
    KIcon icon(item.iconName(), 0, item.overlays());
    QPixmap pixmap = icon.pixmap(500);
    m_previews.insert(item.localPath(), pixmap);
    notifyModelAboutPreview(item.localPath());
}

QPixmap PreviewGenerator::takePreviewPixmap(QString filePath)
{
    QHash<QString, QPixmap>::iterator it = m_previews.find(filePath);
    if(it != m_previews.end()) {
        QPixmap pixmap = it.value();

        // item is not in pending requests
        if (m_pendingItems.find(filePath) == m_pendingItems.end()) {
            m_previews.erase(it);
        }

        return pixmap;
    }

    return m_defaultPreview;
}

void PreviewGenerator::request(const QString &path)
{
    PreviewItemIterator it = m_pendingItems.find(path);
    if (it != m_pendingItems.end()) {
        it.value().count++;
        return;
    }

    KFileItemList fileList;
    KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(path), true);
    fileList.append(fileItem);

    KIO::PreviewJob *job = KIO::filePreview(fileList, QSize(512, 512), &m_plugins);
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);

    PreviewItem item;
    item.job = job;

    m_pendingItems.insert(path, item);

    connect(job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(previewJobResult(const KFileItem&, const QPixmap&)));
    connect(job, SIGNAL(failed(const KFileItem&)), SLOT(previewJobFailed(const KFileItem &)));
}

void PreviewGenerator::cancel(const QString &path)
{
    previewComplete(m_pendingItems.find(path));
    if (m_pendingItems.find(path) == m_pendingItems.end()) {
       m_previews.remove(path);
    }
}
