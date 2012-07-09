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

#include "documentsprovider.h"

#include <KDebug>
#include <KRun>
#include <QApplication>
#include <KRecentDocument>
#include <KDesktopFile>
#include <QFile>

DocumentsProvider::DocumentsProvider(void) : QObject()
{
}

DocumentsProvider::~DocumentsProvider(void)
{
}

void DocumentsProvider::init(void)
{
    reload();
}

void DocumentsProvider::reload(void)
{
    KDesktopFile *desktopFile;
    _clearEntries();
    m_recentDocsList = KRecentDocument::recentDocuments();

    for(int i = 0; i < m_recentDocsList.size(); i++)
    {
        QString fileName = m_recentDocsList[i];
        //kDebug() << fileName;

        if(! KDesktopFile::isDesktopFile(fileName))
            continue;

        if(! QFile::exists(fileName))
            continue;

        desktopFile = new KDesktopFile(fileName);

        if(desktopFile->noDisplay())
        {
            delete desktopFile;
            continue;
        }

        m_recentDocsEntries[desktopFile->readName()] = desktopFile;
    }

}

void DocumentsProvider::_clearEntries(void)
{
    for(QHash<QString, KDesktopFile*>::iterator it = m_recentDocsEntries.begin(); it != m_recentDocsEntries.end(); it++)
        delete it.value();

    m_recentDocsEntries.clear();
}

QStringList DocumentsProvider::getDocsList(void)
{
    return m_recentDocsEntries.keys();
}

void DocumentsProvider::runDoc(const QString &name)
{
    if(! m_recentDocsEntries.contains(name))
        return;

    KRun *krunner = new KRun(KUrl(m_recentDocsEntries[name]->fileName()), QApplication::activeWindow());

    Q_UNUSED(krunner);
}

QString DocumentsProvider::getDocIconName(const QString &name)
{
    if(m_recentDocsEntries.contains(name))
        return m_recentDocsEntries[name]->readIcon();

    return QString();
}


#include "documentsprovider.moc"
