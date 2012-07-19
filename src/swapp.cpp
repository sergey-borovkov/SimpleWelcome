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

#include "appsgridmodel.h"
#include "recentappsgridmodel.h"
#include "favoritesgridmodel.h"
#include "documentsgridmodel.h"
#include "searchgridmodel.h"

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
    m_viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    // Window transparency
    //m_viewer->setAttribute(Qt::WA_TranslucentBackground);
    //m_viewer->setStyleSheet("background:transparent;");

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

    m_viewer->rootContext()->setContextProperty("appsGridModel", new AppsGridModel(this) );
    m_viewer->rootContext()->setContextProperty("recentAppsGridModel", new RecentAppsGridModel(this));
    m_viewer->rootContext()->setContextProperty("favoritesGridModel", new FavoritesGridModel(this));
    m_viewer->rootContext()->setContextProperty("documentsGridModel", new DocumentsGridModel(this));

    m_viewer->rootContext()->setContextProperty("searchCmdGridModel", new SearchFilterGridModel(this, searchGridModel));
    m_viewer->rootContext()->setContextProperty("searchAppsGridModel", new SearchFilterGridModel(this, searchGridModel));
    m_viewer->rootContext()->setContextProperty("searchRecentDocsGridModel", new SearchFilterGridModel(this, searchGridModel));


    m_viewer->showExpanded();
    m_viewer->showFullScreen();
    //m_viewer->setGeometry(896, 0, 800, 600); // 1000); //
    //m_viewer->setFixedSize( m_viewer->sizeHint() );

    QObject::connect((QObject*)m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination

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
