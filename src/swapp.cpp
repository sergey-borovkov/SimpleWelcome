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
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSortFilterProxyModel>

#include <KDebug>
#include <KIcon>
#include <KServiceGroup>

#include "timeframe/activityproxy.h"
#include "timeframe/activityset.h"
#include "timeframe/localdaymodel.h"
#include "timeframe/timescalemodel.h"
#include "timeframe/itemmodel.h"
#include "timeframe/nepomuksource.h"
#include "timeframe/previewgenerator.h"
#include "timeframe/previewprovider.h"
#include "timeframe/social/pluginloader.h"
#include "timeframe/social/socialmodel.h"
#include "timeframe/social/socialproxy.h"

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
    qDebug() << "here";
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
    //m_model = new ActivityModel;

    m_source = new NepomukSource;

    m_nepomukThread = new QThread(this);
    m_source->moveToThread(m_nepomukThread);
    m_nepomukThread->start();
    QRect r = QDesktopWidget().screenGeometry(m_viewer);
    m_viewer->rootContext()->setContextProperty( "desktopWidth", r.width() );

    /* TF Gallery mode */

    LocalDayModel* model = new LocalDayModel;
    TimeFrameDayFilterModel* proxymodel = new TimeFrameDayFilterModel(this);
    proxymodel->setSourceModel(model);
    m_viewer->rootContext()->setContextProperty( "localDayModel", proxymodel );
    m_proxy = new ActivityProxy;
    m_proxy->addNepomukSource(m_source);
    model->setLister(m_proxy);
    m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
    //qmlRegisterUncreatableType<Activity>("Acitivity", 1, 0, "Activity", "Activity is supposed to be used from C++");
   // qmlRegisterUncreatableType<LocalDayItem>("GalleryItem", 1, 0, "Activity", "Activity is supposed to be used from C++");
   // qmlRegisterUncreatableType<LocalDayItem>("GalleryItem", 1, 0, "Activity", "Activity is supposed to be used from C++");
    PreviewGenerator::instance()->setModel(model);
    /* ------------------*/

    /*Time scale model*/
    TimeScaleFilterModel * timeScaleFilterModel = new TimeScaleFilterModel();
    TimeScaleItem* item = new TimeScaleItem();
    TimeScaleModel* timeScaleModel = new TimeScaleModel(item);
    timeScaleFilterModel->setSourceModel(timeScaleModel);
    m_viewer->rootContext()->setContextProperty( "timeScaleModel", timeScaleFilterModel );
    connect(m_proxy,SIGNAL(newMonth(int,int,QString)),timeScaleModel,SLOT(newItem(int,int,QString)));

    /*----------------------------*/


    m_viewer->rootContext()->setContextProperty("documentsProvider", m_documentsProvider);
    m_viewer->rootContext()->setContextProperty( "activityProxy", m_proxy );
    m_viewer->rootContext()->setContextProperty( "nepomukSource", m_source );    

    m_viewer->rootContext()->engine()->addImageProvider("preview", new PreviewProvider);

    //qmlRegisterUncreatableType<ActivitySet>("AcitivitySet", 1, 0, "ActivitySet", "ActivitySet is supposed to be used from C++");



    m_viewer->show();
    //m_viewer->showExpanded();
    m_viewer->setGeometry(100, 000, 640, 480);
    //m_viewer->showFullScreen();

    PluginLoader loader;
    QList<ISocialModule *> plugins = loader.loadPlugins();
    m_manager = new SocialProxy(plugins, this);

    SocialModel *socialModel = new SocialModel;
    m_manager->setModel(socialModel);

    m_viewer->rootContext()->setContextProperty("socialModel", socialModel);

    QObject::connect((QObject*)m_viewer->engine(), SIGNAL(quit()), this, SLOT(quit())); // Temporary solution for app termination
    qDebug() << "THERE";
    if(isLocal())
        m_viewer->setMainQmlFile(QLatin1String("../src/qml/main.qml"));
    else
        m_viewer->setMainQmlFile(QLatin1String("/usr/share/rosa-launcher-qtquick/qml/main.qml"));

    QTimer::singleShot(1000, this, SLOT(init()));

    setQuitOnLastWindowClosed(true); // NEED TO CHANGE TO false

}

SWApp::~SWApp()
{
    delete m_viewer;
    delete m_appProvider;
    delete m_searchRunner;
    delete m_recentAppsProvider;
    delete m_placesProvider;
    delete m_documentsProvider;
    delete m_sessionProvider;
    delete m_userInfoProvider;    
    delete m_proxy;
    delete m_source;
    delete m_manager;
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
