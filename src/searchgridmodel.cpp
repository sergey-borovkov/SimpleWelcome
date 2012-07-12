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
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names;
    names[ CaptionRole ] = "caption";
    names[ ImagePathRole ] = "imagePath";
    names[ GroupRole ] = "group";
    setRoleNames(names);

    m_runnerManager = new Plasma::RunnerManager();

    QStringList activeRunners;
    activeRunners << "recentdocuments"
                  << "shell"
                  << "services";
    m_runnerManager->setAllowedRunners(activeRunners);

    connect(m_runnerManager, SIGNAL(matchesChanged(const QList<Plasma::QueryMatch> &)), this, SLOT(newSearchMatches(const QList<Plasma::QueryMatch> &)));
}

int SearchGridModel::rowCount( const QModelIndex & /*parent*/ ) const
{
    return m_matches.size();
}

QVariant SearchGridModel::data( const QModelIndex &index, int role ) const
{
    if ( index.row() < 0 || index.row() >= m_matches.size() )
        return QVariant();

    QString match = m_matches.keys()[index.row()];

    switch (role)
    {
    case CaptionRole:
        return match;
    case ImagePathRole:
        return QString("image://generalicon/search/%1").arg(match);
    case GroupRole:
        return getMatchGroupName(match);
    }

    return QVariant();
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

QStringList SearchGridModel::getMatchNames()
{
    return m_matches.keys();
}

QStringList SearchGridModel::getGroupNames()
{
    return m_groups.toList();
}

QString SearchGridModel::getMatchGroupName(const QString &name) const
{
    if(m_matches.contains(name))
        return m_matches[name]->runner()->name();

    return "";
}

QIcon SearchGridModel::getMatchIcon(const QString &name)
{
    if(m_matches.contains(name))
        return m_matches[name]->icon();

    return QIcon();
}

void SearchGridModel::runMatch(const QString &name)
{
    if(m_matches.contains(name))
        m_runnerManager->run(*(m_matches[name]));
}


void SearchGridModel::newSearchMatches(const QList<Plasma::QueryMatch> &matches)
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    endRemoveRows();
    _clearMatches();

    for(QList<Plasma::QueryMatch>::const_iterator it = matches.begin(); it != matches.end(); it++)
    {
        m_matches.insert(it->text(), new Plasma::QueryMatch((*it)));
        m_groups.insert(it->runner()->name());
    }

    beginInsertRows(QModelIndex(), 0, rowCount());
    endInsertRows();

    emit newSearchMatchesFound();
}

void SearchGridModel::_clearMatches()
{
    for(QHash<QString, Plasma::QueryMatch*>::iterator it = m_matches.begin(); it != m_matches.end(); it++)
        delete it.value();

    m_matches.clear();
    m_groups.clear();
}

void SearchGridModel::launchSearch(const QString &text)
{
    //kDebug() << "launchSearch called with: " << text;
    if(text.size() > 0)
        m_runnerManager->launchQuery(text);
    else
    {
        m_runnerManager->reset();
    }
}

SearchFilterGridModel::SearchFilterGridModel(QObject *parent, QAbstractListModel *sourceModel)
    : QSortFilterProxyModel(parent)
{
    setFilterRole(SearchGridModel::GroupRole);
    setSourceModel(sourceModel);
}

bool SearchFilterGridModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    return sourceModel()->index(sourceRow, 0, sourceParent).data(SearchGridModel::GroupRole).toString() == group;
}


void SearchFilterGridModel::itemClicked(int newIndex)
{
    QString match = mapToSource(index(newIndex, 0)).data(SearchGridModel::CaptionRole).toString();
    static_cast<SearchGridModel *>(sourceModel())->runMatch(match);
}
