/* KLook
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

#include "previewprovider.h"
#include "previewgenerator.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QDebug>
PreviewProvider::PreviewProvider(const QString &type) :
    QDeclarativeImageProvider(Pixmap),
    m_type(type),
    m_generator(previewGenerator(m_type))
{
    m_defaultPreview.load(":/pla-empty-box.png");
}

QPixmap PreviewProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString str = id.left(id.lastIndexOf('%'));
    bool rounded = false;
    QString round = str.right(8);
    if (round == "/rounded") {
        rounded = true;
        str.chop(8);
    }

    QPixmap pixmap = m_generator->takePreviewPixmap(str);

    if(pixmap.isNull()) {
        QSize size = requestedSize.isValid() ? requestedSize : QSize(512, 512);
        m_generator->request(str, size);
        return m_defaultPreview;
    }

    // only scale down
    if (requestedSize.isValid() && (requestedSize.width() < pixmap.width() || requestedSize.height() < pixmap.height())) {
        pixmap = pixmap.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    }

    if (size)
        *size = pixmap.size();
    if (rounded)
        return QPixmap::fromImage(getRoundedImage(pixmap.toImage(), 10));

    return pixmap;
}



QImage PreviewProvider::getRoundedImage(QImage image, int radius)
{
    QImage out(image.width(), image.height(), QImage::Format_ARGB32);
    out.fill(Qt::transparent);

    QBrush brush(image);

    QPen pen;
    pen.setColor(Qt::transparent);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(0);

    QPainter painter(&out);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRoundedRect(0, 0, image.width(), image.height(), radius, radius , Qt::RelativeSize);

    return out;
}
