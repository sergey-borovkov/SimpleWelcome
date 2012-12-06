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
#include "shadowblur/shadowblur.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>

PreviewProvider::PreviewProvider(const QString &type) :
    QDeclarativeImageProvider(Pixmap),
    m_type(type),
    m_defaultPreview(":/item-placeholder.png"),
    m_generator(previewGenerator(m_type))
{
}

QPixmap PreviewProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString path = id;
    bool rounded = false;
    const QString round = path.right(8);
    if (round == "/rounded") {
        rounded = true;
        path.chop(8);
    }

    QPixmap pixmap = m_generator->takePreviewPixmap(path);

    if (pixmap.isNull()) {
        const QSize previewSize = requestedSize.isValid() ? requestedSize : QSize(512, 512);
        m_generator->request(path, previewSize);

        if (size) {
            pixmap = m_defaultPreview.scaled(previewSize, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation);
            *size = pixmap.size();
        }

        return pixmap;
    }

    if (rounded)
        pixmap = QPixmap::fromImage(getRoundedImage(pixmap.toImage(), 3));

    if (size)
        *size = pixmap.size();

    return pixmap;
}



QImage PreviewProvider::getRoundedImage(QImage image, int radius)
{
    const QSize iconSize = image.size();
    QPixmap pix(iconSize);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    QBrush brush(QPixmap::fromImage(image));
    QPen pen;

    pen.setColor(Qt::transparent);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(0);

    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    p.setBrush(brush);
    p.setPen(pen);
    p.translate((iconSize.width() - image.width()) / 2, (iconSize.height() - image.height()) / 2);
    p.drawRoundedRect(0, 0, image.width() - 1, image.height() - 1, radius, radius, Qt::AbsoluteSize);
    p.end();

    return drawShadowToPixmap(pix, 5, QColor(0, 0, 0, 80), 0, 2);;
}
