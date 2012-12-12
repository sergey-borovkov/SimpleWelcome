/*
 * Copyright (c) ROSA Laboratory 2011-2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
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

#include <KDE/Plasma/RunnerManager>
#include <KDE/KDesktopFile>
#include <KDE/KStandardDirs>

#include <QtCore/QDir>


namespace
{

static QString _FindFullPathToDesktopFile(const QString &desktop_file)
{
    QString path = KStandardDirs::locate("xdgdata-apps", desktop_file);
    if (path.isEmpty())
        path = KStandardDirs::locate("apps", desktop_file);
    if (path.isEmpty())
        path = KStandardDirs::locate("services", desktop_file);
    if (path.isEmpty())
        path = KStandardDirs::locate("servicetypes", desktop_file);
    return path;
}

} // namespace


DataSource_Search::DataSource_Search(QObject *parent, DataSource_RecentApps *inRecentApps)
    : DataSource(parent), recentApps(inRecentApps)
{
    m_runnerManager = new Plasma::RunnerManager();

    QStringList activeRunners;
    activeRunners << "shell"
                  << "recentdocuments"
                  << "services";
    m_runnerManager->setAllowedRunners(activeRunners);
    connect(m_runnerManager, SIGNAL(matchesChanged(const QList<Plasma::QueryMatch> &)), this, SLOT(newSearchMatches(const QList<Plasma::QueryMatch> &)));
}

// Taken from kde-workspace-4.8.2/plasma/desktop/applets/kickoff/core/krunnermodel.cpp, line 92
KService::Ptr serviceForUrl(const KUrl &url)
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
    return matches[group].size();
}

QString DataSource_Search::itemUrlDnd(int id, QString group)
{
    if (id < 0)
        return QString();

    if (matches.contains(group) && id < matches[group].size()) {
        Plasma::QueryMatch &match = *matches[group][id]["plasmaMatch"].value<Plasma::QueryMatch *>();
        QString runner_id = match.runner()->id();
        QString value = match.data().toString();

        if (runner_id == QString::fromAscii("recentdocuments")) {
            // RecentDocuments runner give us path to temporary desktop file
            // we should extract path to recent document (file) and return it
            if (KDesktopFile::isDesktopFile(value)) {
                KDesktopFile file(value);
                QString url = file.readUrl();
                if (!url.isEmpty())
                    return url;
            }
            return value;
        }

        if (KDesktopFile::isDesktopFile(value)) {
            if (QDir::isAbsolutePath(value))
                return QString::fromAscii("file://") + value;
            // try to find absolute path
            QString path = _FindFullPathToDesktopFile(value);
            // Workaround: kde4-name.desktop may be kde4/name.desktop in a real world
            if (path.isEmpty() && value.startsWith(QString::fromAscii("kde4-"))) {
                value[4] = QChar::fromAscii('/');
                path = _FindFullPathToDesktopFile(value);
            }

            if (!path.isEmpty())
                return QString::fromAscii("file://") + path;
        }
        return value;
    }

    return QString();
}

QList<QPair<QString, QString> > DataSource_Search::getRunnersNames()
{
    QStringList activeRunners;
    activeRunners << "shell"
                  << "recentdocuments"
                  << "services";

    QList<QPair<QString, QString> > out;
    foreach(QString allowedRunner, m_runnerManager->allowedRunners()) {
        out << qMakePair(allowedRunner, m_runnerManager->runnerName(allowedRunner));
        //qDebug() << "NAME:" << out.last().first << out.last().second;
    }

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

QIcon DataSource_Search::getMatchIcon(const QString &caption)
{
    foreach (AppItemList matchesList, matches)
        for (int i = 0; i < matchesList.size(); i++)
            if (matchesList[i]["caption"].toString() == caption)
                return matchesList[i]["plasmaMatch"].value<Plasma::QueryMatch*>()->icon();

    return QIcon();
}

void DataSource_Search::itemClicked(int newIndex, QString group)
{
    if (newIndex == -1 || !matches.contains(group))
        return;

    if (newIndex < matches[group].size()) {
        Plasma::QueryMatch &match = *matches[group][newIndex]["plasmaMatch"].value<Plasma::QueryMatch *>();
        QString url = QString("krunner://") + match.runner()->id() + "/" + match.id();

        // Since krunner:// urls can't be added to recent applications,
        // we find the local .desktop entry.

        KService::Ptr service = serviceForUrl(url);
        if (service)
            recentApps->addRecentApp(service->entryPath());
        else {
            //qWarning() << "Failed to find service for" << url; <-- It's just that found entry in not app
        }

        m_runnerManager->run(match);
        emit runDesktopFile("");
    }
}

QVariantMap DataSource_Search::getContent(int index, const QString &group) const
{
    QMap<QString, AppItemList>::const_iterator it = matches.constFind(group);
    if (it != matches.constEnd())
        return it.value()[index];
    return QVariantMap();
}

void DataSource_Search::newSearchMatches(const QList<Plasma::QueryMatch> &newMatches)
{
    QString group;
    for (int i = 0, counter = 0; i < newMatches.size(); i++, counter++) {
        QString currentGroup = newMatches.at(i).runner()->name();

        if (group != currentGroup) {
            group = currentGroup;
            counter = 0;
        }

        QVariant v;
        v.setValue(new Plasma::QueryMatch(newMatches.at(i))); // FIXME: MEMORY LEAK

        AppItem newItem;

        newItem["imagePath"] = QString("image://generalicon/search/%1").arg(newMatches.at(i).text());
        newItem["caption"] = newMatches.at(i).text();
        newItem["id"] = counter;
        newItem["plasmaMatch"] = v;
        newItem["group"] = currentGroup;
        newItem["description"] = newMatches.at(i).subtext();

        matches[currentGroup].append(newItem);
    }

    emit resetContent();
}

void DataSource_Search::launchSearch(const QString &text)
{
    matches.clear();

    if (text.size() > 0)
        m_runnerManager->launchQuery(text);
    else
        m_runnerManager->reset();
}

