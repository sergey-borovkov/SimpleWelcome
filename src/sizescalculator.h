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

#pragma once

#include <QObject>
#include <QSize>

class QmlApplicationViewer;

class SizesCalculator : public QObject
{
    Q_OBJECT

    // Constants
    Q_PROPERTY(int textToGridSpacing READ textToGridSpacing CONSTANT)
    Q_PROPERTY(int groupTextHeight READ groupTextHeight CONSTANT)
    Q_PROPERTY(int gridWithGroupsSpacing READ gridWithGroupsSpacing CONSTANT)
    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT)

    // Calculated
    Q_PROPERTY(int cellWidth READ cellWidth NOTIFY iconSizeChanged)
    Q_PROPERTY(int cellHeight READ cellHeight NOTIFY iconSizeChanged)
    Q_PROPERTY(int iconTextSize READ iconTextSize NOTIFY iconSizeChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconSizeChanged)

public:
    SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer);

    int cellWidth() const;
    int cellHeight() const;
    int iconTextSize() const;
    int iconSize() const;

    QSize thumbnailsSize() const;

    int textToGridSpacing() const
    {
        return 16;
    }

    int groupTextHeight() const
    {
        return 24;
    }

    int gridWithGroupsSpacing() const
    {
        return 32;
    }

    int gridColumns() const
    {
        return 7;
    }

signals:
    void iconSizeChanged();

private:
    QmlApplicationViewer *viewer;

    int availableHeight() const;
    int availableWidth() const;
};
