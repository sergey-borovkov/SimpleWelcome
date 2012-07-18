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

#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QMovie>
#include <QThread>
#include <QTimer>

#include <KDebug>
#include <KIcon>
#include <KServiceGroup>

#include "timeframe/activitylist.h"
#include "timeframe/activitymodel.h"
#include "timeframe/activityproxy.h"
#include "timeframe/activityset.h"
#include "timeframe/galleryitem.h"
#include "timeframe/gallerylister.h"
#include "timeframe/gallerymodel.h"
#include "timeframe/itemmodel.h"
#include "timeframe/listmodel.h"
#include "timeframe/nepomuksource.h"
#include "timeframe/previewgenerator.h"
#include "timeframe/previewprovider.h"
#include "timeframe/social/pluginimageprovider.h"
#include "timeframe/social/pluginloader.h"
#include "timeframe/social/socialitem.h"
#include "timeframe/social/socialproxy.h"
#include "timeframe/social/pluginmodel.h"

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
    //m_viewer->setAttribute(Qt::WA_TranslucentBackground);
    //m_viewer->setStyleSheet("background:transparent;");

    m_appProvider = new AppProvider();
    m_appProvider->init();

    qmlRegisterType<AppEntity>("AppEntity", 1, 0, "AppEntity");
    m_viewer->rootContext()->setContextProperty("appProvider", m_appProvider);

    m_searchRunner = new SearchRunner();
    m_searchRunner->init();
    m_viewer->rootContext()->setContextProperty("searchRunner", m_searchRunner);

    m_recentAppsProvider = new RecentAppsProvider();
    m_recentAppsProvider->init();
    m_appProvider->setAppLaunchReciever(m_recentAppsProvider);
    m_viewer->rootContext()->setContextProperty("recentAppsProvider", m_recentAppsProvider);

    m_placesProvider = new PlacesProvider();
    m_placesProvider->init();
    m_viewer->rootContext()->setContextProperty("placesProvider", m_placesProvider);

    m_documentsProvider = new DocumentsProvider();
    m_documentsProvider->init();
    m_viewer->rootContext()->setContextProperty("documentsProvider", m_documentsProvider);

    m_sessionProvider = new SessionProvider();
    m_sessionProvider->init();
    m_viewer->rootContext()->setContextProperty("sessionProvider", m_sessionProvider);

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

    /*TF*/
    m_model = new ActivityModel;
    m_proxy = new ActivityProxy;
    m_source = new NepomukSource;
    m_nepomukThread = new QThread(this);
    m_source->moveToThread(m_nepomukThread);
    m_nepomukThread->start();

    /* TF Gallery mode */

    GalleryModel* model = new GalleryModel;
    m_viewer->rootContext()->setContextProperty( "galleryModel", model );
    GalleryLister* lister = new GalleryLister;
    lister->addNepomukSource(m_source);
    model->setLister(lister);
    m_viewer->rootContext()->setContextProperty( "galleryLister", lister );
    qmlRegisterUncreatableType<Activity>("Acitivity", 1, 0, "Activity", "Activity is supposed to be used from C++");
    qmlRegisterUncreatableType<GalleryItem>("GalleryItem", 1, 0, "Activity", "Activity is supposed to be used from C++");
    PreviewGenerator::instance()->setModel(model);
    /* ------------------*/

    m_proxy->addSource( m_source );
    m_model->addProxy(m_proxy);

    /*----------------------------*/


    m_viewer->rootContext()->setContextProperty("documentsProvider", m_documentsProvider);
    m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
    m_viewer->rootContext()->setContextProperty( "nepomukSource", m_source );
    m_viewer->rootContext()->setContextProperty( "activityModel", m_model );

    m_viewer->rootContext()->engine()->addImageProvider("preview", new PreviewProvider);

    qmlRegisterUncreatableType<ActivitySet>("AcitivitySet", 1, 0, "ActivitySet", "ActivitySet is supposed to be used from C++");
    qmlRegisterUncreatableType<ActivityList>("ActivityList", 1, 0, "ActivityList", "ActivityList is supposed to be used from C++");


    m_viewer->show();
    //m_viewer->showExpanded();
    m_viewer->setGeometry(100, 000, 640, 480);
    //m_viewer->showFullScreen();

    PluginLoader loader;
    QList<ISocialPlugin *> plugins = loader.loadPlugins();
    m_manager = new SocialProxy(plugins, this);

    m_viewer->rootContext()->setContextProperty("socialModel", m_manager->socialModel());
    m_viewer->rootContext()->setContextProperty("pluginModel", m_manager->pluginModel());
    m_viewer->rootContext()->setContextProperty("socialProxy", m_manager);
    m_viewer->engine()->addImageProvider("plugin", new PluginImageProvider(plugins));


    QObject::connect((QObject*)m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination

    if(isLocal())
        m_viewer->setMainQmlFile(QLatin1String("../src/qml/main.qml"));
    else
        m_viewer->setMainQmlFile(QLatin1String("/usr/share/rosa-launcher-qtquick/qml/main.qml"));

    QTimer::singleShot(1000, this, SLOT(init()));

    setQuitOnLastWindowClosed(true); // NEED TO CHANGE TO false

}

SWApp::~SWApp()
{
    /*
    delete m_viewer;
    delete m_appProvider;
    delete m_searchRunner;
    delete m_recentAppsProvider;
    delete m_placesProvider;
    delete m_documentsProvider;
    delete m_sessionProvider;
    delete m_userInfoProvider;
    delete m_model;
    delete m_proxy;
    delete m_source;
    delete m_manager;
    */
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
