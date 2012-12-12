/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
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

#pragma once

#include "appitem.h"
#include "datasource.h"

#include <QtGui/QIcon>


namespace Plasma
{
class RunnerManager;
class QueryMatch;
}

struct MatchResults {
    QString group;
    QString name;
    Plasma::QueryMatch *plasmaMatch;
};

class DataSource_RecentApps;

class DataSource_Search : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)


public:
    explicit DataSource_Search(QObject *parent = 0, DataSource_RecentApps *recentApps = 0);
    Q_INVOKABLE virtual int getItemCount(QString group = "");
    Q_INVOKABLE virtual QVariantMap getContent(int index, const QString &group = "") const;

    Q_INVOKABLE QString itemUrlDnd(int id, QString group);
    QList<QPair<QString, QString> > getRunnersNames();


public slots:
    void newSearchMatches(const QList<Plasma::QueryMatch> &newMatches);
    void launchSearch(const QString &text);

    void setSearchQuery(const QString &queryText);
    QString getSearchQuery();

    QIcon getMatchIcon(const QString &caption);


    virtual void itemClicked(int newIndex, QString group);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void runDesktopFile(QString desktopFile);


    void currentTabIndexChanged(int newCurrentIndex);

private:
    QString m_searchQuery;
    Plasma::RunnerManager *m_runnerManager;

    QMap<QString, AppItemList> matches;

    DataSource_RecentApps *recentApps;
};


Q_DECLARE_METATYPE(Plasma::QueryMatch *)
