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

#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include <KServiceGroup>
#include <KDebug>
#include <KIcon>

#include "timeframe/activitymodel.h"
#include "timeframe/nepomuksource.h"


SWApp* SWApp::self()
{
    if (!kapp) {
        return new SWApp();
    }

    return qobject_cast<SWApp*>(kapp);
}

SWApp::SWApp()
    : KApplication(),
      m_inited(false)
{

    m_viewer = new QmlApplicationViewer();
    m_viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    m_appProvider = new AppProvider();
    m_appProvider->init();

    qmlRegisterType<AppEntity>("AppEntity", 1, 0, "AppEntity");
    m_viewer->rootContext()->setContextProperty("appProvider", m_appProvider);

    m_appIconProvider = new AppIconProvider();
    m_viewer->engine()->addImageProvider(QLatin1String("appicon"), m_appIconProvider);

    m_viewer->showExpanded();

    QString appPath = applicationFilePath();


    m_model = new ActivityModel();
    m_source = new NepomukSource;
    m_model->addSource( m_source );

    m_viewer->rootContext()->setContextProperty( "activityModel", m_model );


    if(appPath.startsWith("/usr/bin") || appPath.startsWith("/usr/local/bin"))
        m_viewer->setMainQmlFile(QLatin1String("/usr/share/rosa-launcher-qtquick/qml/main.qml"));
    else
        m_viewer->setMainQmlFile(QLatin1String("/home/julia/work/timeframe/src/qml/main.qml"));

    QTimer::singleShot(1000, this, SLOT(init()));

    //setQuitOnLastWindowClosed(false);
}

SWApp::~SWApp()
{
    delete m_viewer;
    delete m_appProvider;
    delete m_appIconProvider;
    m_viewer = NULL;
    m_appProvider = NULL;
    m_appIconProvider = NULL;


    delete m_model;
    delete m_source;


}

int SWApp::newInstance()
{
    return 0;
}


bool SWApp::event(QEvent *event)
{
    return KApplication::event(event);
}

void SWApp::init(void)
{
    if(m_inited)
        return;

    m_inited = true;

}

#include "swapp.moc"
