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

#include "placesprovider.h"

#include <KDebug>
#include <QString>
#include <QList>
#include <KFilePlacesModel>
#include <KRun>
#include <QApplication>

PlacesProvider::PlacesProvider(void) : QObject()
{
}

PlacesProvider::~PlacesProvider(void)
{
}

void PlacesProvider::init(void)
{
    reload();
}

void PlacesProvider::reload(void)
{
    m_placesNames.clear();
    m_placesUrls.clear();
    m_placesIcons.clear();

    KFilePlacesModel *places = new KFilePlacesModel();

    for(int i = 0; i < places->rowCount(); i++)
    {
        QVariant variantName = places->index(i, 0).data(Qt::DisplayRole);
        QVariant variantUrl = places->index(i, 0).data(KFilePlacesModel::UrlRole);
        QVariant variantIcon = places->index(i, 0).data(Qt::DecorationRole);

        //m_hiddenPlaces.append(variantUrl.toString());
        //button = new AppButton(variantIcon.value<QIcon>(), variantName.toString(), variantUrl.toString(), variantUrl.toString());
        //gridWidgetGroup->append(button);

        m_placesNames.append(variantName.toString());
        m_placesUrls.append(variantUrl.toString());
        m_placesIcons.append(variantIcon.value<QIcon>());
    }

    delete places;
}

QStringList PlacesProvider::getPlacesList(void)
{
    return m_placesNames;
}

void PlacesProvider::runPlace(const QString &name)
{
    if(! m_placesNames.contains(name))
        return;

    KRun *krunner = new KRun(KUrl(m_placesUrls[m_placesNames.indexOf(name)]), QApplication::activeWindow());

    Q_UNUSED(krunner);
}

QIcon PlacesProvider::getPlaceIcon(const QString &name)
{
    if(m_placesNames.contains(name))
        return m_placesIcons[m_placesNames.indexOf(name)];

    return QIcon();
}


#include "placesprovider.moc"
