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
#include "previewprovider.h"

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
    m_videoPixmap.load(":/play-empty.png");
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
    const quintptr id = qobject_cast<KJob *>(sender())->property("id").toULongLong();
    QPixmap pict = pixmap;
    if (item.mimetype() == "video/x-mng") { //Kde creates incorect preview for mng files
        QPixmap p(item.localPath());
        // add shadow
        p = QPixmap::fromImage(PreviewProvider::getRoundedImage(p.toImage(), 3));
        m_previews.insert(id, p);
        notifyModelAboutPreview(item.localPath());
        return;
    } else if (item.mimetype().startsWith("video/")) {
        QPainter p(&pict);
        QPixmap scaledPixmap = m_videoPixmap.scaled(pict.width() / 2, pict.height() / 2,
                                                  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (scaledPixmap.size().width() > m_videoPixmap.size().width()
                && scaledPixmap.size().height() > m_videoPixmap.size().height())
            scaledPixmap = m_videoPixmap;

        p.drawPixmap(pict.width() / 2 - scaledPixmap.width() / 2,
                     pict.height() / 2 - scaledPixmap.height() / 2 ,  scaledPixmap);
    }

    // add shadow
    pict = QPixmap::fromImage(PreviewProvider::getRoundedImage(pict.toImage(), 3));

    m_previews.insert(id, pict);
    notifyModelAboutPreview(item.localPath());
}

void PreviewGenerator::previewJobFailed(const KFileItem &item)
{
    KIO::PreviewJob *job = qobject_cast<KIO::PreviewJob *>(sender());
    const QSize size = job->property("requestedSize").toSize();
    KIcon icon(item.iconName(), 0, item.overlays());
    QPixmap pixmap = icon.pixmap(icon.actualSize(size));

    const quintptr id = job->property("id").toInt();
    m_previews.insert(id, pixmap);

    notifyModelAboutPreview(item.localPath());
}

void PreviewGenerator::previewJobFinished(KJob *job)
{
    for(QHash<quintptr, KJob*>::iterator it = m_runningJobs.begin(),
        end = m_runningJobs.end(); it != end; ++it) {
        if(it.value() == job) {
            m_runningJobs.erase(it);
            return;
        }
    }
}

QPixmap PreviewGenerator::takePreviewPixmap(quintptr requestId)
{
    QHash<quintptr, QPixmap>::iterator it = m_previews.find(requestId);
    if (it != m_previews.end()) {
        QPixmap pixmap = it.value();
        m_previews.erase(it);
        return pixmap;
    }

    return QPixmap();
}

void PreviewGenerator::request(const QString &path, const QSize &size, quintptr id)
{
    KFileItemList fileList;
    KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, KUrl(path), true);
    fileList.append(fileItem);

    QHash<quintptr, KJob*>::iterator it(m_runningJobs.find(id));
    if (it != m_runningJobs.end()) {
        //  we found job for this path, let's check if we still can use it
        const QSize jobRequestedSize = it.value()->property("requestedSize").toSize();
        if (jobRequestedSize != size) {
            it.value()->kill();
            m_runningJobs.erase(it);
            m_previews.remove(id);
        }
        else {
            // let's just wait for this result
            return;
        }
    }

    KIO::PreviewJob *job = KIO::filePreview(fileList, size, &m_plugins);
    job->setProperty("requestedSize", size);
    job->setProperty("id", id);
    job->setIgnoreMaximumSize();
    m_runningJobs.insert(id, job);

    connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(previewJobResult(KFileItem,QPixmap)));
    connect(job, SIGNAL(failed(KFileItem)), SLOT(previewJobFailed(KFileItem)));
    connect(job, SIGNAL(result(KJob*)), SLOT(previewJobFinished(KJob*)));
}

void PreviewGenerator::cancel(const QString &requestId)
{
    QString component = requestId;
    //remove ')'
    component.chop(1);
    component = component.mid(component.indexOf('(') + 1);

    const quintptr id = component.toULongLong(0, 16);
    QHash<quintptr, KJob*>::iterator it(m_runningJobs.find(id));
    if(it != m_runningJobs.end()) {
        it.value()->kill();
        m_runningJobs.erase(it);
    }
    QHash<quintptr, QPixmap>::iterator previewIterator(m_previews.find(id));
    if(previewIterator != m_previews.end()) {
        m_previews.erase(previewIterator);
    }
}
