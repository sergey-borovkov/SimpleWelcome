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

#include "datasource_apps.h"
#include "datasource_recentapps.h"
#include "datasource_favorites.h"
#include "datasource_documents.h"
#include "datasource_search.h"

#include "timeframe/timescalemodel.h"
#include "timeframe/local/activityset.h"
#include "timeframe/local/localdayitem.h"
#include "timeframe/local/localcontentmodel.h"
#include "timeframe/local/itemmodel.h"
#include "timeframe/local/activityproxy.h"
#include "timeframe/local/nepomuksource.h"
#include "timeframe/local/previewgenerator.h"
#include "timeframe/local/previewprovider.h"
#include "timeframe/social/pluginloader.h"
#include "timeframe/social/socialproxy.h"
#include "timeframe/social/pluginimageprovider.h"
#include "timeframe/social/socialitem.h"
#include "timeframe/social/pluginmodel.h"
#include "timeframe/social/socialdaymodel.h"
#include "timeframe/social/socialdayitem.h"

#include <listitem.h>

#include "config.h"
#include <KCmdLineArgs>
#include <KRun>
#include <KConfigGroup>
#include <KAction>
#include <QGraphicsDropShadowEffect>

#include <klocalizedstring.h>

SWApp* SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp*>(kapp);
}


QString SWApp::pathToRoot()
{
    QDir root_dir = QCoreApplication::applicationDirPath();
    root_dir.cdUp(); // skip 'bin' directory
    return root_dir.canonicalPath();
}

int SWApp::newInstance()
{
    qDebug() << "New instance here";

    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

    static bool isFirst = true;
    if (!args->isSet("silent") || !isFirst) {
        qDebug() << "Silent not set";
        if (m_viewer) {
            if (!m_viewer->isVisible())
                m_viewer->restore();
            else
                m_viewer->close();
        }
    }
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
    qDebug() << m_globalShortcut.toString() << " is " << m_globalAction->isGlobalShortcutEnabled();
}

SWApp::SWApp()
    : KUniqueApplication()
{
    qDebug() << "constructor";

    // First of all, we have to register type to make it available from QML
    qmlRegisterType<WheelArea>("Private", 0, 1, "WheelArea");

    m_globalAction = new KAction(this);

    m_viewer = new QmlApplicationViewer();
    m_viewer->setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
    m_viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    // Window transparency
    m_viewer->setAttribute(Qt::WA_TranslucentBackground);
    m_viewer->setStyleSheet("background:transparent;");

    m_viewer->rootContext()->setContextProperty("mainWindow", m_viewer);
    m_viewer->rootContext()->setContextProperty("sessionProvider", new SessionProvider(this));

    QMLConstants *constants = new QMLConstants(this, m_viewer);

    UserInfoProvider *userInfoProvider = new UserInfoProvider(this);
    m_viewer->rootContext()->setContextProperty("userInfoProvider", userInfoProvider);

    DataSource_RecentApps *recentAppsDataSource = new DataSource_RecentApps(this);
    m_viewer->rootContext()->setContextProperty("dataSource_RecentApps", recentAppsDataSource);
    connect(recentAppsDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    DataSource_Apps *appsDataSource = new DataSource_Apps(this, recentAppsDataSource);
    m_viewer->rootContext()->setContextProperty("dataSource_Apps", appsDataSource);
    connect(appsDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    DataSource_Favorites *favoritesDataSource = new DataSource_Favorites(this);
    m_viewer->rootContext()->setContextProperty("dataSource_Favorites", favoritesDataSource);
    connect(favoritesDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    DataSource_Documents *docsDataSource = new DataSource_Documents(this, constants);
    m_viewer->rootContext()->setContextProperty("dataSource_Documents", docsDataSource);
    connect(docsDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));
    connect(m_viewer, SIGNAL(windowShown()), docsDataSource, SLOT(updateContent()));

    DataSource_Search *searchDataSource = new DataSource_Search(this, recentAppsDataSource);
    m_viewer->rootContext()->setContextProperty("searchGridModel", searchDataSource);
    connect(searchDataSource, SIGNAL(runDesktopFile(QString)), SLOT(runDesktopFile(QString)));

    m_viewer->rootContext()->setContextProperty("constants", constants);
    m_viewer->rootContext()->setContextProperty("i18n_Welcome", i18n("Welcome"));
    m_viewer->rootContext()->setContextProperty("i18n_Applications", i18n("Applications"));
    m_viewer->rootContext()->setContextProperty("i18n_TimeFrame", i18n("TimeFrame"));

    m_viewer->rootContext()->setContextProperty("i18n_Recent_Applications", i18n("Recent Applications"));
    m_viewer->rootContext()->setContextProperty("i18n_Favorites", i18n("Places"));
    m_viewer->rootContext()->setContextProperty("i18n_Recent_Documents", i18n("Recent Documents"));

    m_viewer->rootContext()->setContextProperty("i18n_All", i18n("All"));
    m_viewer->rootContext()->setContextProperty("i18n_Photo", i18n("Photo"));
    m_viewer->rootContext()->setContextProperty("i18n_Video", i18n("Video"));
    m_viewer->rootContext()->setContextProperty("i18n_Documents", i18n("Documents"));
    m_viewer->rootContext()->setContextProperty("i18n_Manage_networks", i18n("Manage networks"));
    m_viewer->rootContext()->setContextProperty("i18n_My_Local_Documents", i18n("My Local Documents"));
    m_viewer->rootContext()->setContextProperty("i18n_Local", i18n("Local"));
    m_viewer->rootContext()->setContextProperty("i18n_Write_Comment", i18n("Write comment..."));
    m_viewer->rootContext()->setContextProperty("i18n_Audio", i18n("Audio: "));
    m_viewer->rootContext()->setContextProperty("i18n_Like", i18n("Like"));
    m_viewer->rootContext()->setContextProperty("i18n_Unlike", i18n("Unlike"));
    m_viewer->rootContext()->setContextProperty("i18n_Social_networkong_sites", i18n("Social networking sites"));
    m_viewer->rootContext()->setContextProperty("i18n_Show_Comments", i18n("Show comments"));
    m_viewer->rootContext()->setContextProperty("i18n_Hide_Comments", i18n("Hide comments"));
    m_viewer->rootContext()->setContextProperty("i18n_Login", i18n("Login"));
    m_viewer->rootContext()->setContextProperty("i18n_Logout", i18n("Logout"));

    m_viewer->rootContext()->setContextProperty("i18n_JAN", i18n("JAN"));
    m_viewer->rootContext()->setContextProperty("i18n_FEB", i18n("FEB"));
    m_viewer->rootContext()->setContextProperty("i18n_MAR", i18n("MAR"));
    m_viewer->rootContext()->setContextProperty("i18n_APR", i18n("APR"));
    m_viewer->rootContext()->setContextProperty("i18n_MAY", i18n("MAY"));
    m_viewer->rootContext()->setContextProperty("i18n_JUN", i18n("JUN"));
    m_viewer->rootContext()->setContextProperty("i18n_JUL", i18n("JUL"));
    m_viewer->rootContext()->setContextProperty("i18n_AUG", i18n("AUG"));
    m_viewer->rootContext()->setContextProperty("i18n_SEP", i18n("SEP"));
    m_viewer->rootContext()->setContextProperty("i18n_OCT", i18n("OCT"));
    m_viewer->rootContext()->setContextProperty("i18n_NOV", i18n("NOV"));
    m_viewer->rootContext()->setContextProperty("i18n_DEC", i18n("DEC"));

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

    m_viewer->setMainQmlFile(pathToRoot() + QString::fromLatin1("/" SW_QML_PATH "/main.qml"));   // Qt converts path to native automatically

    setQuitOnLastWindowClosed(true); // NEED TO CHANGE TO false
}

void SWApp::initTimeframeLocalMode()
{
    m_source = new NepomukSource;
    m_nepomukThread = new QThread(this);
    m_source->moveToThread(m_nepomukThread);
    m_nepomukThread->start(QThread::LowPriority);

    m_proxy = new ActivityProxy;
    m_proxy->addNepomukSource(m_source);

    LocalContentModel* model = new LocalContentModel(LocalDayItem::roleNames(), this);
    LocalContentFilterModel* proxymodel = new LocalContentFilterModel(this);
    model->setLister(m_proxy);
    proxymodel->setSourceModel(model);
    PreviewGenerator::instance()->setModel(model);

    QRect r = QDesktopWidget().screenGeometry(m_viewer);

    m_viewer->rootContext()->setContextProperty("desktopWidth", r.width());
    m_viewer->rootContext()->setContextProperty("localDayModel", proxymodel);
    m_viewer->rootContext()->setContextProperty("activityProxy", m_proxy);
    m_viewer->rootContext()->setContextProperty("nepomukSource", m_source);

    m_viewer->rootContext()->engine()->addImageProvider("preview", new PreviewProvider);
}

void SWApp::initTimeframeSocialMode()
{
    PluginLoader loader;
    QList<ISocialPlugin *> plugins = loader.loadPlugins(pathToRoot() + QString::fromLatin1("/" SW_TIMEFRAME_PLUGINS_PATH));

    SocialDayModel* socialModel = new SocialDayModel(SocialDayItem::roleNames());
    SocialDayFilterModel* socialProxyModel = new SocialDayFilterModel(this);
    socialProxyModel->setSourceModel(socialModel);
    m_manager = new SocialProxy(plugins, this);
    m_manager->setSocialModel(socialModel);

    m_viewer->engine()->addImageProvider("plugin", new PluginImageProvider(plugins));

    TimeScaleFilterModel * timeScaleFilterModel = new TimeScaleFilterModel();
    TimeScaleModel* timeScaleModel = new TimeScaleModel(TimeScaleItem::roleNames(), this);
    timeScaleFilterModel->setSourceModel(timeScaleModel);

    connect(m_manager, SIGNAL(removeType(QString)), timeScaleModel, SLOT(removeItems(QString)));

    m_viewer->rootContext()->setContextProperty("socialProxy", m_manager);
    m_viewer->rootContext()->setContextProperty("socialModel", m_manager->socialModel());
    m_viewer->rootContext()->setContextProperty("socialDayModel", socialProxyModel);
    m_viewer->rootContext()->setContextProperty("pluginModel", m_manager->pluginModel());
    m_viewer->rootContext()->setContextProperty("timeScaleModel", timeScaleFilterModel);
    m_viewer->rootContext()->setContextProperty("previewGenerator", PreviewGenerator::instance());

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
    m_viewer->close();
}

void SWApp::globalActionTriggered()
{
    if (m_viewer->isHidden())
        m_viewer->restore();
    else
        m_viewer->close();
}

QMLConstants::QMLConstants(QObject *parent, QmlApplicationViewer *inViewer)
    : QObject(parent), viewer(inViewer)
{
    connect(viewer, SIGNAL(windowSizeChanged(int, int)), SIGNAL(iconSizeChanged()));
}

int QMLConstants::cellWidth()
{
    return viewer->updatableHeight() >= 1080 ? 140 :
           viewer->updatableHeight() >= 1024 ? 130 :
           viewer->updatableHeight() >= 900 ? 120 :
           viewer->updatableHeight() >= 850 ? 110 :
           viewer->updatableHeight() >= 800 ? 110 :
           viewer->updatableHeight() >= 750 ? 100 :
           70;
}

int QMLConstants::cellHeight()
{
    //return (viewer->updatableHeight() - 80 /*topBar*/ - 80 /*bottomBar*/ - 80 - (textToGridSpacing() + groupTextHeight())*3 - gridWithGroupsSpacing()*2) / 4 - 1;
    return viewer->updatableHeight() >= 1080 ? 180 :
           viewer->updatableHeight() >= 1024 ? 165 :
           viewer->updatableHeight() >= 900 ? 130 :
           viewer->updatableHeight() >= 850 ? 120 :
           viewer->updatableHeight() >= 800 ? 110 :
           viewer->updatableHeight() >= 750 ? 100 :
           80;
}

int QMLConstants::iconTextSize()
{
    return viewer->updatableHeight() >= 1080 ? 10 : viewer->updatableHeight() >= 1024 ? 10 : viewer->updatableHeight() >= 600 ? 9 : 8;
}

int QMLConstants::iconSize()
{
    return viewer->updatableHeight() >= 1080 ? 96 :
           viewer->updatableHeight() >= 1024 ? 80 :
           viewer->updatableHeight() >= 900 ? 64 :
           viewer->updatableHeight() >= 800 ? 56 :
           viewer->updatableHeight() >= 768 ? 48 : 32;
}
