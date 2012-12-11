/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#include "pluginloader.h"
#include <socialplugin.h>

#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QString>

QList<ISocialPlugin *> PluginLoader::loadPlugins(QString dir_path)
{
    QDir modulesDir(dir_path);
    QList<ISocialPlugin *> plugins;

    foreach(QString fileName, modulesDir.entryList(QDir::AllEntries)) {
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *libObject = loader.instance();
        if (ISocialPlugin *lib = qobject_cast<ISocialPlugin *>(libObject))
            plugins << lib;
    }

    return plugins;
}
