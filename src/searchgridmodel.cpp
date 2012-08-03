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

#include "searchgridmodel.h"

#include <KDebug>
#include <Plasma/RunnerManager>
#include <Plasma/QueryMatch>
#include <KDesktopFile>
#include <QStringList>
#include <QFile>
#include <KRecentDocument>

SearchGridModel::SearchGridModel(QObject *parent)
    : DataSource(parent)
{
    m_runnerManager = new Plasma::RunnerManager();

    QStringList activeRunners;
    activeRunners << "recentdocuments"
                  << "shell"
                  << "services";
    m_runnerManager->setAllowedRunners(activeRunners);

    connect(m_runnerManager, SIGNAL(matchesChanged(const QList<Plasma::QueryMatch> &)), this, SLOT(newSearchMatches(const QList<Plasma::QueryMatch> &)));
    qDebug() << connect(m_runnerManager, SIGNAL(queryFinished()), SLOT(test2()));
}

int SearchGridModel::getItemCount(QString group)
{
    int count = 0;
    for (int i = 0; i < matches.size(); i++)
        if (matches[i].group == group)
            count++;
    return count;
}




void SearchGridModel::setSearchQuery(const QString &queryText)
{
    m_searchQuery = queryText;

    emit currentTabIndexChanged(queryText.isEmpty());

    launchSearch(queryText);
}

QString SearchGridModel::getSearchQuery()
{
    return m_searchQuery;
}

QIcon SearchGridModel::getMatchIcon(const QString &name)
{
    for (int i= 0; i < matches.size(); i++)
        if(matches[i].name == name)
            return matches[i].plasmaMatch->icon();

    return QIcon();
}

void SearchGridModel::runMatch(const QString &name)
{
    //if(m_matches.contains(name))
    //    m_runnerManager->run(*(m_matches[name]));
}

void SearchGridModel::itemClicked(int newIndex)
{
    if (newIndex != -1)
        m_runnerManager->run(*matches[newIndex].plasmaMatch);
}

void SearchGridModel::updateContent()
{
    for (int i = 0; i < matches.size(); i++)
    {
        emit newItemData(QString("image://generalicon/search/%1").arg(matches[i].name), matches[i].name, i, matches[i].group);
    }
}

void SearchGridModel::test2()
{
    qDebug() << "WE ARE HERE!";
}

void SearchGridModel::newSearchMatches(const QList<Plasma::QueryMatch> &newMatches)
{
    for (int i = 0; i < newMatches.size(); i++)
    {
        matches.resize(matches.size() + 1);
        matches.last().name = newMatches.at(i).text();
        matches.last().group = newMatches.at(i).runner()->name();
        matches.last().plasmaMatch = new Plasma::QueryMatch(newMatches.at(i));
        //emit newItemData(QString("image://generalicon/search/%1").arg(matches.last().name), matches.last().name, matches.size() - 1, matches.last().group);
    }

    emit newSearchMatchesFound();
}

void SearchGridModel::launchSearch(const QString &text)
{
    //for(QHash<QString, Plasma::QueryMatch*>::iterator it = m_matches.begin(); it != m_matches.end(); it++)
    //    delete it.value();
    /// FIXFIXFIX

    matches.clear();
    //emit dataClear();



    if(text.size() > 0)
        m_runnerManager->launchQuery(text);
    else
    {
        m_runnerManager->reset();
    }
}

