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

#include <QtCore/QFileInfo>
#include <QtCore/QRect>
#include <QtCore/QStringList>
#include <QtGui/QLabel>
#include <QtGui/QPainter>

#include <kfile.h>
#include <kicon.h>

PreviewGenerator *PreviewGenerator::m_instance = 0;

PreviewGenerator::PreviewGenerator()
{
    defaultPreview.load(":/pla-empty-box.png");
    videoPixmap.load(":/play-empty.png");
    m_plugins = KIO::PreviewJob::availablePlugins();
}

void PreviewGenerator::notifyModelAboutPreview(const QString &url)
{
    QHash<QString, LocalDayFilterModel *>::iterator it = m_urlsInModel.find(url);
    if(it != m_urlsInModel.end()) {
        it.value()->previewReady(url);
        m_urlsInModel.erase(it);
    }
}

void PreviewGenerator::previewJobResult(const KFileItem &item, const QPixmap &pixmap)
{
    if(m_urlsInModel.find(item.localPath()) == m_urlsInModel.end())
        return;

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
    if(m_urlsInModel.find(item.localPath()) == m_urlsInModel.end())
        return;

    KIcon icon(item.iconName(), 0, item.overlays());
    QPixmap pixmap = icon.pixmap(500);
    m_previews.insert(item.localPath(), pixmap);
    notifyModelAboutPreview(item.localPath());
}

PreviewGenerator * PreviewGenerator::instance()
{
    if (!m_instance)
        m_instance = new PreviewGenerator;
    return m_instance;
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

void PreviewGenerator::modelShown(QStringList paths, QObject *dayModel)
{
    LocalDayFilterModel *filteredModel = static_cast<LocalDayFilterModel *>(dayModel);
    KFileItemList fileList;
    foreach(QString path, paths) {
        KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(path), true);
        fileList.append(fileItem);
        m_urlsInModel.insert(path, filteredModel);
    }

    KIO::PreviewJob *job = KIO::filePreview(fileList, QSize(512, 512), &m_plugins);
    job->setIgnoreMaximumSize();
    job->setAutoDelete(true);

    connect(job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(previewJobResult(const KFileItem&, const QPixmap&)));
    connect(job, SIGNAL(failed(const KFileItem&)), SLOT(previewJobFailed(const KFileItem &)));
}

void PreviewGenerator::modelHidden(QStringList paths, QObject *dayModel)
{
    LocalDayFilterModel *filteredModel = static_cast<LocalDayFilterModel *>(dayModel);
    Q_UNUSED(filteredModel)
    foreach(QString path, paths) {
        m_urlsInModel.remove(path);
    }
}
