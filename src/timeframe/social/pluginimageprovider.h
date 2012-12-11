/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
 *  Evgeniy Auzhin <evgeniy.augin@osinit.ru>
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

#ifndef PLUGINIMAGEPROVIDER_H
#define PLUGINIMAGEPROVIDER_H

#include <socialplugin.h>

#include <QtDeclarative/QDeclarativeImageProvider>

class PluginImageProvider : public QDeclarativeImageProvider
{
public:
    PluginImageProvider(const QList<ISocialPlugin *> &plugins)
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
          m_plugins(plugins) {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
        QString s = id;
        bool isSmall = false;
        if (s.right(6) == "/small") {
            s.chop(6);
            isSmall = true;
        }
        foreach(ISocialPlugin * plugin, m_plugins) {
            if (plugin->name() == s) {
                QPixmap p;
                if (isSmall) {
                    p = plugin->smallIcon();
                } else {
                    p = plugin->icon();
                }
                if (requestedSize.isValid()) {
                    p = p.scaled(*size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    if (size)
                        *size = p.size();
                }
                return p;
            }
        }

        return QPixmap();
    }
private:
    const QList<ISocialPlugin *> m_plugins;
};

#endif // PLUGINIMAGEPROVIDER_H
