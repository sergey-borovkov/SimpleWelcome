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

#include "wheelarea.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include <QDesktopWidget>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtDeclarative/QtDeclarative>
#include <KServiceGroup>

#include "generaliconprovider.h"
#include "sessionprovider.h"
#include "userinfoprovider.h"
#include "sizescalculator.h"

#include "datasource_apps.h"
#include "datasource_recentapps.h"
#include "datasource_favorites.h"
#include "datasource_documents.h"
#include "datasource_search.h"

#include "timeframe/timescalemodel.h"
#include "timeframe/local/localcontentitem.h"
#include "timeframe/local/localcontentmodel.h"
#include "timeframe/local/localdaymodel.h"
#include "timeframe/local/activityproxy.h"
#include "timeframe/local/nepomuksource.h"
#include "timeframe/local/previewgenerator.h"
#include "timeframe/local/previewprovider.h"
#include "timeframe/social/pluginloader.h"
#include "timeframe/social/socialproxy.h"
#include "timeframe/social/pluginimageprovider.h"
#include "timeframe/social/socialitem.h"
#include "timeframe/social/pluginmodel.h"
#include "timeframe/social/socialcontentmodel.h"
#include "timeframe/social/socialdayitem.h"

#include <listitem.h>

#include "config.h"
#include <KCmdLineArgs>
#include <KRun>
#include <KConfigGroup>
#include <KAction>
#include <QGraphicsDropShadowEffect>

#include <klocalizedstring.h>

SWApp *SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp *>(kapp);
}


QString SWApp::pathToRoot()
{
    QDir root_dir = QCoreApplication::applicationDirPath();
    root_dir.cdUp(); // skip 'bin' directory
    return root_dir.canonicalPath();
}

int SWApp::newInstance()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    static bool isFirst = true;
    if (!args->isSet("silent") || !isFirst) {
        if (m_viewer) {
            if (!m_viewer->isVisible())
                m_viewer->show();
            else
                m_viewer->hide();
        }
    }
    else if (!isFirst)
        qDebug() << "Silent is set";

    isFirst = false;
    return 0;
}

#include <kglobalaccel.h>


void SWApp::loadShortcut()
{
    m_globalShortcut = KShortcut("Alt+F1");

    KGlobalAccel::stealShortcutSystemwide(QKeySequence("Alt+F1"));

    connect(m_globalAction, SIGNAL(triggered(bool)), SLOT(newInstance()));
    m_globalAction->setObjectName("Activate A ROSA Launcher Widget");
    m_globalAction->setGlobalShortcut(m_globalShortcut, KAction::ActiveShortcut, KAction::NoAutoloading);
}

#include <QMainWindow>

SWApp::SWApp()
    : KUniqueApplication()
    , m_dataSourceApps(0)
{
    // First of all, we have to register type to make it available from QML
    qmlRegisterType<WheelArea>("Private", 0, 1, "WheelArea");

    m_globalAction = new KAction(this);

    m_viewer = new QmlApplicationViewer();

    connect(m_viewer, SIGNAL(windowHidden()), SLOT(windowHidden()));
    connect(m_viewer, SIGNAL(windowShown()), SLOT(windowShown()));

    m_kdeclarative.setDeclarativeEngine(m_viewer->engine());
    m_kdeclarative.initialize();
    m_kdeclarative.setupBindings();

    m_viewer->rootContext()->setContextProperty("mainWindow", m_viewer);
    m_viewer->rootContext()->setContextProperty("sessionProvider", new SessionProvider(this));

    SizesCalculator *constants = new SizesCalculator(this, m_viewer);
    m_viewer->setSizesCalculator(constants);

    UserInfoProvider *userInfoProvider = new UserInfoProvider(this);
    m_viewer->rootContext()->setContextProperty("userInfoProvider", userInfoProvider);

    DataSource_RecentApps *recentAppsDataSource = new DataSource_RecentApps(this);
    m_viewer->rootContext()->setContextProperty("dataSource_RecentApps", recentAppsDataSource);
    connect(recentAppsDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));
    connect(m_viewer, SIGNAL(windowShown()), recentAppsDataSource, SLOT(checkApps()));

    m_dataSourceApps = new DataSource_Apps(this, recentAppsDataSource);
    m_viewer->rootContext()->setContextProperty("dataSource_Apps", m_dataSourceApps);
    connect(m_dataSourceApps, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    DataSource_Favorites *favoritesDataSource = new DataSource_Favorites(this);
    m_viewer->rootContext()->setContextProperty("dataSource_Favorites", favoritesDataSource);
    connect(favoritesDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    DataSource_Documents *docsDataSource = new DataSource_Documents(this, constants);
    m_viewer->rootContext()->setContextProperty("dataSource_Documents", docsDataSource);
    connect(docsDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));
    connect(m_viewer, SIGNAL(windowShown()), docsDataSource, SLOT(updateContent()));
    connect(m_viewer, SIGNAL(iconSizeChanged()), docsDataSource, SLOT(iconSizeChanged()));

    DataSource_Search *searchDataSource = new DataSource_Search(this, recentAppsDataSource);
    m_viewer->rootContext()->setContextProperty("searchGridModel", searchDataSource);
    connect(searchDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    m_viewer->rootContext()->setContextProperty("constants", constants);

    qmlRegisterType<QGraphicsDropShadowEffect>("Effects", 1, 0, "DropShadow");

    QList<QPair<QString, QString> > runners = searchDataSource->getRunnersNames();
    for (int i = 0; i < runners.size(); i++)
        m_viewer->rootContext()->setContextProperty(QString("i18n_%1").arg(runners[i].first), runners[i].second);

    m_generalIconProvider = new GeneralIconProvider(pathToRoot() + QString::fromLatin1("/" SW_ASSETS_PATH "/"), constants);
    m_generalIconProvider->setUserInfoProvider(userInfoProvider);
    m_generalIconProvider->setSearchDataSource(searchDataSource);
    m_generalIconProvider->setDocumentsDataSource(docsDataSource);
    m_viewer->engine()->addImageProvider(QLatin1String("generalicon"), m_generalIconProvider);

    loadShortcut();

    initTimeframeLocalMode();
    initTimeframeSocialMode();

    connect(this, SIGNAL(focusChanged(QWidget *, QWidget *)), m_viewer, SLOT(focusChanged(QWidget *, QWidget *)));
    m_viewer->setSource(QUrl::fromLocalFile(pathToRoot() + QString::fromLatin1("/" SW_QML_PATH "/main.qml")));   // Qt converts path to native automatically

    setQuitOnLastWindowClosed(true); // NEED TO CHANGE TO false
}

void SWApp::initTimeframeLocalMode()
{
    m_source = new NepomukSource;
    m_nepomukThread = new QThread(this);
    m_source->moveToThread(m_nepomukThread);
    m_nepomukThread->start(QThread::LowPriority);

    m_proxy = new ActivityProxy(m_source, this);

    LocalContentModel *model = new LocalContentModel(LocalContentItem::roleNames(), this);
    LocalContentFilterModel *proxymodel = new LocalContentFilterModel(this);
    model->setLister(m_proxy);
    proxymodel->setSourceModel(model);

    QRect r = QDesktopWidget().screenGeometry(m_viewer);
    previewGenerator("gallery")->setModel(model);
    previewGenerator("timeline")->setModel(model);
    m_viewer->rootContext()->setContextProperty("desktopWidth", r.width());
    m_viewer->rootContext()->setContextProperty("localDayModel", proxymodel);
    m_viewer->rootContext()->setContextProperty("activityProxy", m_proxy);
    m_viewer->rootContext()->setContextProperty("nepomukSource", m_source);
    m_viewer->rootContext()->setContextProperty("galleryPreviewGenerator", previewGenerator("gallery"));
    m_viewer->rootContext()->setContextProperty("timelinePreviewGenerator", previewGenerator("timeline"));

    m_viewer->rootContext()->engine()->addImageProvider("timeline", new PreviewProvider("timeline"));
    m_viewer->rootContext()->engine()->addImageProvider("gallery", new PreviewProvider("gallery"));
}

void SWApp::initTimeframeSocialMode()
{
    PluginLoader loader;
    QList<ISocialPlugin *> plugins = loader.loadPlugins(pathToRoot() + QString::fromLatin1("/" SW_TIMEFRAME_PLUGINS_PATH));

    SocialContentModel *socialModel = new SocialContentModel(SocialContentItem::roleNames());
    SocialContentFilterModel *socialProxyModel = new SocialContentFilterModel(this);
    socialProxyModel->setSourceModel(socialModel);
    m_manager = new SocialProxy(plugins, this);
    m_manager->setSocialModel(socialModel);

    m_viewer->engine()->addImageProvider("plugin", new PluginImageProvider(plugins));
    qmlRegisterInterface<LocalDayFilterModel>("LocalDayFilterModel");
    TimeScaleFilterModel *timeScaleFilterModel = new TimeScaleFilterModel();
    TimeScaleModel *timeScaleModel = new TimeScaleModel(TimeScaleItem::roleNames(), this);
    timeScaleFilterModel->setSourceModel(timeScaleModel);
    connect(m_manager, SIGNAL(removeType(QString)), timeScaleModel, SLOT(removeItems(QString)));

    m_viewer->rootContext()->setContextProperty("socialProxy", m_manager);
    m_viewer->rootContext()->setContextProperty("socialModel", m_manager->socialModel());
    m_viewer->rootContext()->setContextProperty("socialDayModel", socialProxyModel);
    m_viewer->rootContext()->setContextProperty("pluginModel", m_manager->pluginModel());
    m_viewer->rootContext()->setContextProperty("timeScaleModel", timeScaleFilterModel);

    connect(m_proxy, SIGNAL(newMonth(int, int, QString)), timeScaleModel, SLOT(newItem(int, int, QString)));
    connect(m_manager, SIGNAL(newMonth(int, int, QString)), timeScaleModel, SLOT(newItem(int, int, QString)));
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

void SWApp::runDesktopFile(QString desktopFile)
{
    if (!desktopFile.isEmpty())
        new KRun(KUrl(desktopFile), QApplication::activeWindow());
    m_viewer->hide();
}

void SWApp::windowHidden()
{
    m_dataSourceApps->setUpdateAllowed(true);
}

void SWApp::windowShown()
{
    m_dataSourceApps->setUpdateAllowed(false);
}
