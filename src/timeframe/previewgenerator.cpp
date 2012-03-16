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

#include "previewgenerator.h"

#include <QtCore/QStringList>
#include <QtCore/QRect>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtCore/QtDebug>
#include <QtCore/QFileInfo>
#include <kfile.h>

PreviewGenerator *PreviewGenerator::m_instance = 0;

PreviewGenerator::PreviewGenerator(QObject *parent) :
    QObject(parent), m_job(0)
{    
    defaultPreview.load(":/pla-empty-box.png");
    m_plugins = KIO::PreviewJob::availablePlugins();
}

void PreviewGenerator::setPreview(const KFileItem &item, const QPixmap &pixmap)
{    
    QPixmap pict = pixmap;
    m_fileList.removeAll(item);
    if(item.mimetype().startsWith("video/"))
    {
        QPainter p(&pict);        
        QPixmap scaledPixmap = videoPixmap.scaled(pict.width()/2, pict.height()/2,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawPixmap(pict.width()/2 - scaledPixmap.width()/2, pict.height()/2 - scaledPixmap.height()/2 ,  scaledPixmap );
    }
    previews.insert(item.localPath(), pict);
}

void PreviewGenerator::jobDeleted()
{
    qDebug() << "JOB DELETED";
}

void PreviewGenerator::setNullIcon(const KFileItem &item)
{
    //qDebug() << "Preview generation failed" << item.localPath() << item.mimetype();

}

QPixmap PreviewGenerator::getPreviewPixmap(QString filePath)
{    
    if(previews.contains(filePath))
        return previews[filePath];
    else {
//        qDebug() << "Default preview";
        return defaultPreview;
    }

}

PreviewGenerator * PreviewGenerator::instance()
{
    if(!m_instance)
        m_instance = new PreviewGenerator;
    return m_instance;
}

void PreviewGenerator::start(const QStringList& list)
{
    for(int i = 0; i < list.size(); i++)
    {
        KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, list[i], false);
        m_fileList.append(fileItem);
    }

    m_job = KIO::filePreview(m_fileList, 1000, 0 , 0, 0, true, true, &m_plugins);
    m_job->setIgnoreMaximumSize();
    m_job->setAutoDelete(true);

    connect(m_job, SIGNAL(gotPreview(const KFileItem&, const QPixmap&)), SLOT(setPreview(const KFileItem&, const QPixmap&)));
    //connect(m_job, SIGNAL(result()), SLOT(deleteJob()));
    connect(m_job, SIGNAL(failed(const KFileItem&)), SLOT(setNullIcon(const KFileItem &)));
    //connect(m_job, SIGNAL(),SLOT(jobDeleted()));
}
