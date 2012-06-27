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

#include "searchrunner.h"

#include <KDebug>
#include <Plasma/RunnerManager>
#include <Plasma/QueryMatch>

SearchRunner::SearchRunner(void) : QObject()
{
}

SearchRunner::~SearchRunner(void)
{
}

void SearchRunner::init(void)
{
    m_runnerManager = new Plasma::RunnerManager();

    QStringList activeRunners;
    activeRunners << "recentdocuments"
                  << "shell"
                  << "rosa-services2";

    m_runnerManager->setAllowedRunners(activeRunners);

    connect(m_runnerManager, SIGNAL(matchesChanged(const QList<Plasma::QueryMatch> &)), this, SLOT(newSearchMatches(const QList<Plasma::QueryMatch> &)));
}

void SearchRunner::setSearchQuery(const QString &queryText)
{
    m_searchQuery = queryText;

    launchSearch(queryText);
}

QString SearchRunner::getSearchQuery(void)
{
    return m_searchQuery;
}

QStringList SearchRunner::getMatchNames(void)
{
    return m_matches.keys();
}

QStringList SearchRunner::getGroupNames(void)
{
    return m_groups.toList();
}

QString SearchRunner::getMatchGroupName(const QString &name)
{
    if(m_matches.contains(name))
        return m_matches[name]->runner()->name();

    return "";
}

QIcon SearchRunner::getMatchIcon(const QString &name)
{
    if(m_matches.contains(name))
        return m_matches[name]->icon();

    return QIcon();
}

void SearchRunner::runMatch(const QString &name)
{
    if(m_matches.contains(name))
        m_runnerManager->run(*(m_matches[name]));
}


void SearchRunner::newSearchMatches(const QList<Plasma::QueryMatch> &matches)
{
    _clearMatches();

    for(QList<Plasma::QueryMatch>::const_iterator it = matches.begin(); it != matches.end(); it++)
    {
        m_matches.insert(it->text(), new Plasma::QueryMatch((*it)));
        m_groups.insert(it->runner()->name());
    }

    emit newSearchMatchesFound();
}

void SearchRunner::_clearMatches(void)
{
    for(QHash<QString, Plasma::QueryMatch*>::iterator it = m_matches.begin(); it != m_matches.end(); it++)
        delete it.value();

    m_matches.clear();
    m_groups.clear();
}

void SearchRunner::launchSearch(const QString &text)
{
    //kDebug() << "launchSearch called with: " << text;
    if(text.size() > 0)
        m_runnerManager->launchQuery(text);
    else
        m_runnerManager->reset();
}

#include "searchrunner.moc"
