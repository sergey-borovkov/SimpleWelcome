/*
 * Copyright (c) ROSA Laboratory 2011
 * License: GPLv3
 *
 * Authors:
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
 *
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

#include "recentappsprovider.h"

#include <QFile>
#include <KDebug>
#include <KDesktopFile>
#include <KRun>
#include <QApplication>
#include <KConfig>
#include <KConfigGroup>
#include <KGlobal>

RecentAppsProvider::RecentAppsProvider(void) : QObject()
{
}

RecentAppsProvider::~RecentAppsProvider(void)
{
    _writeRecentApps();
}

void RecentAppsProvider::init(void)
{
    _readRecentApps();
    _fillEntries();
}

QStringList RecentAppsProvider::getRecentAppsList(void)
{
    return m_recentAppsEntriesList;
}

QString RecentAppsProvider::getRecentAppIconName(const QString &name)
{
    if(m_recentAppsEntries.contains(name))
        return m_recentAppsEntries[name]->readIcon();

    return QString();
}

void RecentAppsProvider::runRecentApp(const QString &name)
{
    if(! m_recentAppsEntries.contains(name))
        return;

    KRun *krunner = new KRun(KUrl(m_recentAppsEntries[name]->fileName()), QApplication::activeWindow());

    Q_UNUSED(krunner);
}

void RecentAppsProvider::registerLaunchedApp(const QString &name)
{
    m_recentAppsList.removeAll(name);
    m_recentAppsList.prepend(name);
    _writeRecentApps();
    QString fullname = _fillEntry(name);

    if(! fullname.isNull())
        m_recentAppsEntriesList.prepend(fullname);
}


void RecentAppsProvider::_readRecentApps(void)
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup *configGroup = new KConfigGroup(config, "General");

    m_recentAppsList = configGroup->readEntry("Recent applications", QStringList());

    //delete config; // Smart pointer used
    delete configGroup;
}

void RecentAppsProvider::_writeRecentApps(void)
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup *configGroup = new KConfigGroup(config, "General");

    configGroup->writeEntry("Recent applications", m_recentAppsList);

    configGroup->sync();

    //delete config; // Smart pointer used
    delete configGroup;
}

void RecentAppsProvider::_clearEntries(void)
{
    for(QHash<QString, KDesktopFile*>::iterator it = m_recentAppsEntries.begin(); it != m_recentAppsEntries.end(); it++)
        delete it.value();

    m_recentAppsEntries.clear();
    m_recentAppsEntriesList.clear();
}

void RecentAppsProvider::_fillEntries(void)
{
    _clearEntries();

    for(int i = 0; i < m_recentAppsList.size(); i++)
    {
        QString fileName = m_recentAppsList[i];
        //kDebug() << fileName;
        QString name = _fillEntry(fileName);

        if(! name.isNull())
            m_recentAppsEntriesList.append(name);
    }
}

QString RecentAppsProvider::_fillEntry(const QString &fileName)
{
    KDesktopFile *desktopFile;

    if(! KDesktopFile::isDesktopFile(fileName))
        return QString();

    if(! QFile::exists(fileName))
        return QString();

    desktopFile = new KDesktopFile(fileName);

    if(desktopFile->noDisplay())
    {
        delete desktopFile;
        return QString();
    }

    m_recentAppsEntries[desktopFile->readName()] = desktopFile;

    return desktopFile->readName();
}

#include "recentappsprovider.moc"
