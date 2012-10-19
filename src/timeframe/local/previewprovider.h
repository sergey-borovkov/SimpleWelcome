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

#ifndef PREVIEWIMAGEPROVIDER_H
#define PREVIEWIMAGEPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

class PreviewGenerator;

class PreviewProvider : public QDeclarativeImageProvider
{
public:
    PreviewProvider(const QString &type);
    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);
private:
    QImage getRoundedImage(QImage image, int radius);
    QString m_type;
    PreviewGenerator *m_generator;
};

#endif // PREVIEWIMAGEPROVIDER_H
