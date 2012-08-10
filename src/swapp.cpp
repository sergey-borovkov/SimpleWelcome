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

#include <KServiceGroup>

#include "timeframe/activityset.h"
#include "timeframe/localdaymodel.h"
#include "timeframe/timescalemodel.h"
#include "timeframe/itemmodel.h"
#include "timeframe/listmodel.h"
#include "timeframe/activityproxy.h"
#include "timeframe/nepomuksource.h"
#include "timeframe/previewgenerator.h"
#include "timeframe/previewprovider.h"
#include "timeframe/social/pluginloader.h"
#include "timeframe/social/socialproxy.h"
#include "timeframe/social/pluginimageprovider.h"
#include "timeframe/social/socialitem.h"
#include "timeframe/social/pluginmodel.h"
#include "timeframe/social/socialdaymodel.h"
#include "timeframe/social/socialdayitem.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#include "config.h"

SWApp* SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp*>(kapp);
}


QString SWApp::pathToShareDir()
{
    QDir root_dir = QCoreApplication::applicationDirPath();
    root_dir.cdUp(); // skip 'bin' directory
    QString path = root_dir.canonicalPath();
    path += QString::fromLatin1("/share/" SW_PROJECT_NAME "/");
    return path;
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

    m_generalIconProvider = new GeneralIconProvider(pathToShareDir() + QString::fromLatin1("assets/"));
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
    initTimeframeLocalMode();
    initTimeframeSocialMode();

    connect(m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination

    m_viewer->setMainQmlFile( pathToShareDir() + QString::fromLatin1("qml/main.qml") ); // Qt converts path to native automatically

    setQuitOnLastWindowClosed( true ); // NEED TO CHANGE TO false
}

void SWApp::initTimeframeLocalMode()
{
    m_source = new NepomukSource;
    m_nepomukThread = new QThread(this);
    m_source->moveToThread(m_nepomukThread);
    m_nepomukThread->start();

    m_proxy = new ActivityProxy;
    m_proxy->addNepomukSource(m_source);

    LocalDayModel* model = new LocalDayModel;
    TimeFrameDayFilterModel* proxymodel = new TimeFrameDayFilterModel(this);
    model->setLister(m_proxy);
    proxymodel->setSourceModel(model);
    PreviewGenerator::instance()->setModel(model);

    QRect r = QDesktopWidget().screenGeometry(m_viewer);

    m_viewer->rootContext()->setContextProperty( "desktopWidth", r.width() );
    m_viewer->rootContext()->setContextProperty( "localDayModel", proxymodel );
    m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
    m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
    m_viewer->rootContext()->setContextProperty( "nepomukSource", m_source );
    m_viewer->rootContext()->engine()->addImageProvider("preview", new PreviewProvider);
}

void SWApp::initTimeframeSocialMode()
{
    PluginLoader loader;
    QList<ISocialPlugin *> plugins = loader.loadPlugins();

    SocialDayModel* socialModel = new SocialDayModel( SocialDayItem::roleNames() );
    SocialDayFilterModel* socialProxyModel = new SocialDayFilterModel( this );
    socialProxyModel->setSourceModel( socialModel );
    m_manager = new SocialProxy( plugins, this );
    m_manager->setModel(socialModel );

    m_viewer->engine()->addImageProvider( "plugin", new PluginImageProvider( plugins ) );

    TimeScaleFilterModel * timeScaleFilterModel = new TimeScaleFilterModel();
    TimeScaleItem* item = new TimeScaleItem();
    TimeScaleModel* timeScaleModel = new TimeScaleModel(item);
    timeScaleFilterModel->setSourceModel(timeScaleModel);

    m_viewer->rootContext()->setContextProperty("socialProxy", m_manager );
    m_viewer->rootContext()->setContextProperty("socialModel", m_manager->socialModel());
    m_viewer->rootContext()->setContextProperty("socialDayModel", socialProxyModel );
    m_viewer->rootContext()->setContextProperty("pluginModel", m_manager->pluginModel());
    m_viewer->rootContext()->setContextProperty("timeScaleModel", timeScaleFilterModel);

    connect(m_proxy,SIGNAL(newMonth(int,int,QString)),timeScaleModel,SLOT(newItem(int,int,QString)));
    connect(m_manager,SIGNAL(newMonth(int,int,QString)),timeScaleModel,SLOT(newItem(int,int,QString)));
}

SWApp::~SWApp()
{
    m_nepomukThread->exit();
    m_nepomukThread->wait(100);
    delete m_viewer;
}

bool SWApp::event(QEvent *event)
{
    return KUniqueApplication::event(event);
}


QMLConstants::QMLConstants(QObject *parent, QmlApplicationViewer *inViewer)
    : QObject(parent), viewer(inViewer)
{
    connect(viewer, SIGNAL(windowSizeChanged(int,int)), SIGNAL(iconSizeChanged()));
}

int QMLConstants::cellWidth()
{
    return viewer->updatableHeight() >= 1080 ? 140 : viewer->updatableHeight() >= 1024 ? 130 : viewer->updatableHeight() >= 600 ? 120 : 70;
}

int QMLConstants::cellHeight()
{
     return viewer->updatableHeight() >= 1080 ? 180 : viewer->updatableHeight() >= 1024 ? 165 : viewer->updatableHeight() >= 600 ? 150 : 100;
}

int QMLConstants::iconTextSize()
{
     return viewer->updatableHeight() >= 1080 ? 10 : viewer->updatableHeight() >= 1024 ? 10 : viewer->updatableHeight() >= 600 ? 9 : 8;
}

int QMLConstants::iconSize()
{
    return viewer->updatableHeight() >= 1080 ? 96 : viewer->updatableHeight() >= 1024 ? 80 : viewer->updatableHeight() >= 600 ? 64 : 32;
}
