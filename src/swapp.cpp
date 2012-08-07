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

#include "swapp.h"
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "generaliconprovider.h"
#include "sessionprovider.h"
#include "userinfoprovider.h"

#include "datasource_apps.h"
#include "datasource_recentapps.h"
#include "datasource_favorites.h"
#include "datasource_documents.h"
#include "searchgridmodel.h"

#include <QDesktopWidget>

SWApp* SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp*>(kapp);
}

SWApp::SWApp()
    : KUniqueApplication()
{
    m_viewer = new QmlApplicationViewer();
    m_viewer->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    m_viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    // Window transparency
    //m_viewer->setAttribute(Qt::WA_TranslucentBackground);
    //m_viewer->setStyleSheet("background:transparent;");
    m_viewer->rootContext()->setContextProperty("mainWindow", m_viewer);

    m_viewer->rootContext()->setContextProperty("sessionProvider", new SessionProvider(this));

    UserInfoProvider *userInfoProvider = new UserInfoProvider(this);
    m_viewer->rootContext()->setContextProperty("userInfoProvider", userInfoProvider);

    SearchGridModel *searchGridModel = new SearchGridModel(this);
    m_viewer->rootContext()->setContextProperty("searchGridModel", searchGridModel);

    m_generalIconProvider = new GeneralIconProvider();
    m_generalIconProvider->setIsLocal(isLocal());
    m_generalIconProvider->setUserInfoProvider(userInfoProvider);
    m_generalIconProvider->setSearchGridModel(searchGridModel);
    m_viewer->engine()->addImageProvider(QLatin1String("generalicon"), m_generalIconProvider);

    DataSource_RecentApps *recentApps = new DataSource_RecentApps(this);
    m_viewer->rootContext()->setContextProperty("dataSource_RecentApps", recentApps);
    m_viewer->rootContext()->setContextProperty("dataSource_Apps", new DataSource_Apps(this, recentApps));
    m_viewer->rootContext()->setContextProperty("dataSource_Favorites", new DataSource_Favorites(this));
    m_viewer->rootContext()->setContextProperty("dataSource_Documents", new DataSource_Documents(this));

    m_viewer->rootContext()->setContextProperty("searchCmdGridModel", searchGridModel);
    m_viewer->rootContext()->setContextProperty("searchAppsGridModel", searchGridModel);
    m_viewer->rootContext()->setContextProperty("searchRecentDocsGridModel", searchGridModel);

    m_viewer->rootContext()->setContextProperty("constants", new QMLConstants(this, m_viewer));

    m_viewer->setGeometry(896, 0, 1280, 1024); // 1000); //
    m_viewer->show();
    //m_viewer->showFullScreen();
    //m_viewer->move(/*896*/0, 0);
    //m_viewer->setFixedSize( m_viewer->sizeHint() );

    connect(m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination

    if(isLocal())
        m_viewer->setMainQmlFile(QLatin1String("../src/qml/main.qml"));
    else
        m_viewer->setMainQmlFile(QLatin1String("/usr/share/rosa-launcher-qtquick/qml/main.qml"));

    setQuitOnLastWindowClosed(true); // NEED TO CHANGE TO false
}

SWApp::~SWApp()
{
    delete m_viewer;
}

bool SWApp::event(QEvent *event)
{
    return KUniqueApplication::event(event);
}

bool SWApp::isLocal()
{
    QString appPath = applicationFilePath();

    if(appPath.startsWith("/usr/bin") || appPath.startsWith("/usr/local/bin"))
        return false;

    return true;
}


QMLConstants::QMLConstants(QObject *parent, QmlApplicationViewer *inViewer)
    : QObject(parent), viewer(inViewer)
{
    connect(viewer, SIGNAL(windowSizeChanged(int,int)), SIGNAL(iconSizeChanged()));
}

int QMLConstants::cellWidth()
{
    return viewer->updatableWidth() >= 1280 ? 140 : viewer->updatableWidth() >= 800 ? 120 : 70;
}

int QMLConstants::cellHeight()
{
     return viewer->updatableWidth() >= 1280 ? 200 : viewer->updatableWidth() >= 800 ? 150 : 100;
}

int QMLConstants::iconTextSize()
{
     return viewer->updatableWidth() >= 1280 ? 10 : viewer->updatableWidth() >= 800 ? 9 : 8;
}

int QMLConstants::iconSize()
{
    return viewer->updatableWidth() >= 1280 ? 96 : viewer->updatableWidth() >= 800 ? 64 : 32;
}
