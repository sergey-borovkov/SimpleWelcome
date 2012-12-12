/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
 * Dmitry Ashkadov <dmitry.ashkadov@rosalab.ru>
 * Alexey Yermakov <alexey.yermakov@rosalab.ru>
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

#pragma once

#include <QtDeclarative/QDeclarativeView>
#include <QtCore/QVariantList>

class QResizeEvent;
class SizesCalculator;

class QmlApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

    Q_PROPERTY(QRect availableGeometry READ availableGeometry NOTIFY availableGeometryChanged)
    Q_PROPERTY(bool visible READ isVisible NOTIFY visibleChanged)

public:
    explicit QmlApplicationViewer(QWidget *parent = 0);

    /// @return available space in screen (relative to screen)
    QRect availableGeometry() const { return m_availGeometry; }

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void moveEvent(QMoveEvent *);

    Q_INVOKABLE void activateDragAndDrop(QString url, QString image_path, int image_size);

    Q_INVOKABLE void saveStacks(QVariantList setting);
    Q_INVOKABLE void saveIconPositions(QVariantMap setting);
    Q_INVOKABLE QVariantMap loadStacks() const;
    Q_INVOKABLE QVariantMap loadIconPositions() const;

    Q_INVOKABLE void currentTabChanged(int newCurrentIndex);

    void setSizesCalculator(SizesCalculator *consts);

signals:
    void availableGeometryChanged();
    void windowHidden();
    void windowShown();
    void iconSizeChanged();
    void visibleChanged();

public slots:
    void updateWorkArea(int screen);
    void onScreenSizeChanged(int screen);
    void focusChanged(QWidget *, QWidget *now);


private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

    int currentTabIndex;
    int m_screen;
    QRect m_availGeometry;
    SizesCalculator *constants;
};
