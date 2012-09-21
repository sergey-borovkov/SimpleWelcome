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

#include "datasource_search.h"
#include "datasource_recentapps.h"

#include <KUrl>


#include <Plasma/RunnerManager>
#include <KDE/Plasma/QueryMatch>
#include <KDE/KDesktopFile>
#include <QStringList>
#include <QFile>
#include <KRecentDocument>

DataSource_Search::DataSource_Search(QObject *parent, DataSource_RecentApps *inRecentApps)
    : DataSource(parent), recentApps(inRecentApps)
{
    m_runnerManager = new Plasma::RunnerManager();

    QStringList activeRunners;
    activeRunners << "shell"
                  << "recentdocuments"
                  << "services";

    connect(m_runnerManager, SIGNAL(matchesChanged(const QList<Plasma::QueryMatch> &)), this, SLOT(newSearchMatches(const QList<Plasma::QueryMatch> &)));
    connect(m_runnerManager, SIGNAL(queryFinished()), SLOT(test2()));
}

// Taken from kde-workspace-4.8.2/plasma/desktop/applets/kickoff/core/krunnermodel.cpp, line 92
KService::Ptr serviceForUrl(const KUrl & url)
{
    QString runner = url.host();
    QString id = url.path();

    if (id.startsWith(QLatin1Char('/'))) {
        id = id.remove(0, 1);
    }

    if (runner != QLatin1String("services")) {
        return KService::Ptr(NULL);
    }

    // URL path example: services_kde4-kate.desktop
    // or: services_firefox.desktop
    id.remove("services_");

    return KService::serviceByStorageId(id);
}

int DataSource_Search::getItemCount(QString group)
{
    int count = 0;
    for(int i = 0; i < matches.size(); i++)
        if(matches[i].group == group)
            count++;

    return count;
}

QString DataSource_Search::itemUrlDnd(int id, QString group)
{
    if (id < 0)
        return QString();

    for (int i = 0, sz = matches.size(); i < sz; i++) {
        if (matches[i].group == group) {
            if (i + id < sz) {
                Plasma::QueryMatch& match = *matches[i + id].plasmaMatch;
                QString value = match.data().toString().toUtf8();
                if (KDesktopFile::isDesktopFile(value)) {
                    KDesktopFile file(value);
                    QString url = file.readUrl();
                    if (!url.isEmpty())
                        return url;
                }
                return value;
            }
            break;
        }
    }

    return QString();
}

QList<QPair<QString, QString> > DataSource_Search::getRunnersNames()
{
    QList<QPair<QString, QString> > out;
    foreach (QString allowedRunner, m_runnerManager->allowedRunners())
        out << qMakePair(allowedRunner, m_runnerManager->runnerName(allowedRunner));

    return out;
}




void DataSource_Search::setSearchQuery(const QString &queryText)
{
    m_searchQuery = queryText;

    emit currentTabIndexChanged(queryText.isEmpty());

    launchSearch(queryText);
}

QString DataSource_Search::getSearchQuery()
{
    return m_searchQuery;
}

QIcon DataSource_Search::getMatchIcon(const QString &name)
{
    for(int i = 0; i < matches.size(); i++)
        if(matches[i].name == name)
            return matches[i].plasmaMatch->icon();

    return QIcon();
}

void DataSource_Search::itemClicked(int newIndex, QString group)
{
    if (newIndex == -1)
        return;

    for (int i = 0; i < matches.size(); i++)
    {
        if (matches[i].group == group)
        {
            if (i + newIndex < matches.size())
            {
                Plasma::QueryMatch& match = *matches[i + newIndex].plasmaMatch;
                QString url = QString("krunner://") + match.runner()->id() + "/" + match.id();

                // Since krunner:// urls can't be added to recent applications,
                // we find the local .desktop entry.

                KService::Ptr service = serviceForUrl(url);
                if (service)
                    recentApps->addRecentApp(service->desktopEntryPath());
                else
                    qWarning() << "Failed to find service for" << url;

                m_runnerManager->run(*matches[i + newIndex].plasmaMatch);
                emit runDesktopFile("");
            }
            break;
        }
    }
}

void DataSource_Search::getContent()
{
    QString group;
    for(int i = 0, counter = 0; i < matches.size(); i++) {
        if(group != matches[i].group) {
            group = matches[i].group;
            counter = 0;
        }
        emit newItemData(QString("image://generalicon/search/%1").arg(matches[i].name), matches[i].name, counter, matches[i].group);
        counter++;
    }
}

void DataSource_Search::test2()
{
    qDebug() << "WE ARE HERE!";
}

void DataSource_Search::newSearchMatches(const QList<Plasma::QueryMatch> &newMatches)
{
    for (int i = 0; i < newMatches.size(); i++)
    {
        matches.append(MatchResults());
        matches.last().name = newMatches.at(i).text();
        matches.last().group = newMatches.at(i).runner()->name();
        matches.last().plasmaMatch = new Plasma::QueryMatch(newMatches.at(i));
        //emit newItemData(QString("image://generalicon/search/%1").arg(matches.last().name), matches.last().name, matches.size() - 1, matches.last().group);
        //qDebug() << "FOUND" << matches.last().name << "FROM" << matches.last().group;
    }
    emit resetContent();
}

void DataSource_Search::launchSearch(const QString &text)
{
    //for(QHash<QString, Plasma::QueryMatch*>::iterator it = m_matches.begin(); it != m_matches.end(); it++)
    //    delete it.value();
    /// FIXFIXFIX

    matches.clear();

    if(text.size() > 0)
        m_runnerManager->launchQuery(text);
    else {
        m_runnerManager->reset();
    }
}

