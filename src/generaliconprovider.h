/*
 * Copyright (c) ROSA Laboratory 2012
 * License: GPLv3
 *
 * Authors:
 * Alexander Skakov <alexander.skakov@osinit.ru>
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

#include <QDeclarativeImageProvider>

#include <QtCore/QString>

class UserInfoProvider;
class DataSource_Search;
class DataSource_Documents;
class SizesCalculator;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider(QString path_to_assets, SizesCalculator *constants);

    void setUserInfoProvider(UserInfoProvider *userInfoProvider) {
        m_userInfoProvider = userInfoProvider;
    }
    void setSearchDataSource(DataSource_Search *searchDataSource) {
        m_searchDataSource = searchDataSource;
    }
    void setDocumentsDataSource(DataSource_Documents *documentsDataSource) {
        m_documentsDataSource = documentsDataSource;
    }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    UserInfoProvider *m_userInfoProvider;
    DataSource_Search *m_searchDataSource;
    DataSource_Documents *m_documentsDataSource;
    SizesCalculator *constants;
    QString m_pathToAssets;
};
