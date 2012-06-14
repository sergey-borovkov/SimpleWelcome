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

#include <QTimer>
#include <QThread>

#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include <QMovie>

#include <KServiceGroup>
#include <KDebug>
#include <KIcon>

#include "timeframe/activityproxy.h"
#include "timeframe/nepomuksource.h"
#include "timeframe/activitylist.h"
#include "timeframe/activityset.h"
#include "timeframe/previewprovider.h"
#include "timeframe/activitymodel.h"
#include "timeframe/social/pluginloader.h"

SWApp* SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp*>(kapp);
}

SWApp::SWApp()
  : KUniqueApplication(),
    m_inited(false)
{
  m_viewer = new QmlApplicationViewer();
  m_viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

  // Window transparency
  m_viewer->setAttribute(Qt::WA_TranslucentBackground);
  m_viewer->setStyleSheet("background:transparent;");
  
  //kDebug() << QMovie::supportedFormats();
  
  //m_viewer->addImportPath("/usr/lib/kde4/imports/");
  
  m_appProvider = new AppProvider();
  m_appProvider->init();

  qmlRegisterType<AppEntity>("AppEntity", 1, 0, "AppEntity");
  m_viewer->rootContext()->setContextProperty("appProvider", m_appProvider);

  m_searchRunner = new SearchRunner();
  m_searchRunner->init();
  m_viewer->rootContext()->setContextProperty("searchRunner", m_searchRunner);
  
  m_appIconProvider = new AppIconProvider();
  m_viewer->engine()->addImageProvider(QLatin1String("appicon"), m_appIconProvider);

  m_recentAppsProvider = new RecentAppsProvider();
  m_recentAppsProvider->init();
  m_appProvider->setAppLaunchReciever(m_recentAppsProvider);
  m_viewer->rootContext()->setContextProperty("recentAppsProvider", m_recentAppsProvider);

  m_model = new ActivityModel;
  m_proxy = new ActivityProxy;
  m_source = new NepomukSource;
  m_nepomukThread = new QThread(this);
  m_source->moveToThread(m_nepomukThread);
  m_nepomukThread->start();

  m_proxy->addSource( m_source );

  m_model->addProxy(m_proxy);


  m_placesProvider = new PlacesProvider();
  m_placesProvider->init();
  m_viewer->rootContext()->setContextProperty("placesProvider", m_placesProvider);

  m_documentsProvider = new DocumentsProvider();
  m_documentsProvider->init();
  m_viewer->rootContext()->setContextProperty("documentsProvider", m_documentsProvider);
  m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
  m_viewer->rootContext()->setContextProperty( "nepomukSource", m_source );
  m_viewer->rootContext()->setContextProperty( "activityModel", m_model );

  m_viewer->rootContext()->engine()->addImageProvider("preview", new PreviewProvider);

  qmlRegisterUncreatableType<ActivitySet>("AcitivitySet", 1, 0, "ActivitySet", "ActivitySet is supposed to be used from C++");
  qmlRegisterUncreatableType<ActivityList>("ActivityList", 1, 0, "ActivityList", "ActivityList is supposed to be used from C++");

  m_userInfoProvider = new UserInfoProvider();
  m_userInfoProvider->init();
  m_viewer->rootContext()->setContextProperty("userInfoProvider", m_userInfoProvider);
  
  m_generalIconProvider = new GeneralIconProvider();
  m_generalIconProvider->setIsLocal(isLocal());
  m_generalIconProvider->setSearchRunner(m_searchRunner);
  m_generalIconProvider->setRecentAppsProvider(m_recentAppsProvider);
  m_generalIconProvider->setPlacesProvider(m_placesProvider);
  m_generalIconProvider->setDocumentsProvider(m_documentsProvider);
  m_generalIconProvider->setUserInfoProvider(m_userInfoProvider);
  m_viewer->engine()->addImageProvider(QLatin1String("generalicon"), m_generalIconProvider);

  m_viewer->show();
  //m_viewer->showExpanded();
  //m_viewer->showFullScreen();
  PluginLoader loader;
  loader.loadPlugins();

  QObject::connect((QObject*)m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination
  
  if(isLocal())
    m_viewer->setMainQmlFile(QLatin1String("../src/qml/main.qml"));
  else
    m_viewer->setMainQmlFile(QLatin1String("/usr/share/rosa-launcher-qtquick/qml/main.qml"));

  QTimer::singleShot(1000, this, SLOT(init()));
  
  setQuitOnLastWindowClosed(true);
}

SWApp::~SWApp()
{
  delete m_viewer;
  delete m_appProvider;
  delete m_appIconProvider;
  delete m_searchRunner;
  delete m_generalIconProvider;
  delete m_recentAppsProvider;
  delete m_placesProvider;
  delete m_documentsProvider;
  delete m_sessionProvider;
  delete m_userInfoProvider;
  m_viewer = NULL;
  m_appProvider = NULL;
  m_appIconProvider = NULL;
  m_searchRunner = NULL;
  m_generalIconProvider = NULL;
  m_recentAppsProvider = NULL;
  m_placesProvider = NULL;
  m_documentsProvider = NULL;
  m_sessionProvider = NULL;
  m_userInfoProvider = NULL;
}

int SWApp::newInstance()
{
    return 0;
}


bool SWApp::event(QEvent *event)
{
  return KUniqueApplication::event(event);
}

void SWApp::init(void)
{
  if(m_inited)
    return;
  
  m_inited = true;
  
}

bool SWApp::isLocal(void)
{
  QString appPath = applicationFilePath();
  
  if(appPath.startsWith("/usr/bin") || appPath.startsWith("/usr/local/bin"))
    return false;
  
  return true;
}

#include "swapp.moc"
