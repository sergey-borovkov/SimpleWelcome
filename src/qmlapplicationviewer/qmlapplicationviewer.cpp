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

#include "qmlapplicationviewer.h"
#include "../sizescalculator.h"

#include <KDE/KWindowSystem>
#include <KDE/Plasma/WindowEffects>
#include <KDE/Plasma/Theme>

#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QResizeEvent>


QmlApplicationViewer::QmlApplicationViewer(QWidget *parent) :
    QDeclarativeView(parent),
    currentTabIndex(0),
    m_screen(0),
    constants(NULL)
{
    QDesktopWidget *desktop = QApplication::desktop();
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    connect(desktop, SIGNAL(workAreaResized(int)), SLOT(updateWorkArea(int)));
    connect(desktop, SIGNAL(resized(int)), SLOT(onScreenSizeChanged(int)));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setWindowFlags(Qt::FramelessWindowHint);

    m_screen = desktop->screenNumber(QCursor::pos());
    setGeometry(desktop->screenGeometry(m_screen));
    setFixedSize(size());

    // Window transparency
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");
}

void QmlApplicationViewer::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);
    updateWorkArea(m_screen);
}


void QmlApplicationViewer::updateWorkArea(int screen)
{
    if (screen == m_screen) {
        QRect screen_geom = QApplication::desktop()->screenGeometry(screen);
        QRect avail_geom = QApplication::desktop()->availableGeometry(screen);

        // compute rect relative to screen
        avail_geom.translate(-screen_geom.left(), -screen_geom.top());

        if (m_availGeometry != avail_geom) {
            m_availGeometry = avail_geom;
            emit availableGeometryChanged();

            if (constants) {
                static int iconSizeWas = 0;
                if (iconSizeWas != constants->iconSize()) {
                    iconSizeWas = constants->iconSize();
                    emit iconSizeChanged();
                }
            }
        }
    }
}

void QmlApplicationViewer::onScreenSizeChanged(int screen)
{
    if (screen == m_screen) {
        // resize main window to fill all screen
        QRect geom = QApplication::desktop()->screenGeometry(screen);
        move(geom.topLeft());
        setFixedSize(geom.size()); // we should fix size of window
    }
}

void QmlApplicationViewer::focusChanged(QWidget *, QWidget *now)
{
    if (!now && currentTabIndex != 3) // When not on TimeFrame tab
        hide();
}

void QmlApplicationViewer::showEvent(QShowEvent *event)
{
    QDeclarativeView::showEvent(event);

    // compute screen index
    int new_screen = QApplication::desktop()->screenNumber(QCursor::pos());
    if (new_screen != m_screen) {
        // resize if SW should be shown on another screen
        m_screen = new_screen;
        onScreenSizeChanged(m_screen);
    }

    KWindowSystem::setState(winId(), NET::SkipTaskbar);

    emit visibleChanged();
    emit windowShown();

    activateWindow();
}

void QmlApplicationViewer::hideEvent(QHideEvent *event)
{
    QDeclarativeView::hideEvent(event);
    emit visibleChanged();
    emit windowHidden();
}

void QmlApplicationViewer::moveEvent(QMoveEvent *)
{
    QPoint required_pos = QApplication::desktop()->screenGeometry(m_screen).topLeft();
    if (required_pos != pos())
        move(required_pos); // disallow moving of window by user
}


void QmlApplicationViewer::closeEvent(QCloseEvent *event)
{
    // doesn't close the window, instead hide it
    event->ignore();
    hide();
}

void QmlApplicationViewer::activateDragAndDrop(QString url, QString image_path, int image_size)
{
    QDeclarativeEngine *eng = engine();

    QMimeData *mime = new QMimeData;
    KUrl::List urls;
    urls.append(KUrl(url));
    urls.populateMimeData(mime);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);

    if (eng) {
        image_path.remove(QString::fromLatin1("image://"));
        // grep name of image provider
        QString provider_name = image_path.section(QChar::fromAscii('/'), 0, 0);
        QString image_id = image_path.section(QChar::fromAscii('/'), 1);
        QDeclarativeImageProvider *provider = eng->imageProvider(provider_name);
        if (provider && provider->imageType() == QDeclarativeImageProvider::Pixmap) {
            QSize size;
            QPixmap pixmap = provider->requestPixmap(image_id, &size, QSize(image_size, image_size));
            drag->setPixmap(pixmap);
        }
    }

    drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, Qt::CopyAction);
}

void QmlApplicationViewer::saveStacks(QVariantList setting)
{
    KConfigGroup configGroup(KGlobal::config(), "Stacks");
    configGroup.deleteGroup();

    foreach(QVariant variant, setting) {
        QVariantMap map;
        if (!strcmp(variant.typeName(), "QVariantMap")) {
            map = variant.value<QVariantMap>();

            QStringList stackList;
            QVariantList stack = map["stack"].toList();
            foreach(QVariant item, stack) {
                QVariantMap properties = item.toMap();
                stackList.append(properties["caption"].toString());
            }

            configGroup.writeEntry(map["caption"].toString(), stackList);
        }
    }

    configGroup.sync();
}

void QmlApplicationViewer::saveIconPositions(QVariantMap setting)
{
    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    configGroup.deleteGroup();

    QVariantMap::const_iterator it = setting.constBegin();
    for (; it != setting.constEnd(); ++it) {
        configGroup.writeEntry(it.key(), it.value());
    }

    configGroup.sync();
}

QVariantMap QmlApplicationViewer::loadStacks() const
{
    QVariantMap list;

    KConfigGroup configGroup(KGlobal::config(), "Stacks");
    QMap<QString, QString > map = configGroup.entryMap();

    foreach(QString key, map.keys())
        list[key] = map[key];

    return list;
}


QVariantMap QmlApplicationViewer::loadIconPositions() const
{
    QVariantMap out;

    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    QMap<QString, QString> map = configGroup.entryMap();

    QMap<QString, QString>::iterator it = map.begin();
    for (; it != map.end(); ++it)
        out[it.key()] = it.value().toInt();

    return out;
}

void QmlApplicationViewer::currentTabChanged(int newCurrentIndex)
{
    currentTabIndex = newCurrentIndex;
}

void QmlApplicationViewer::setSizesCalculator(SizesCalculator *consts)
{
    constants = consts;
}
