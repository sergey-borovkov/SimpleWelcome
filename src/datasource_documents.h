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

#include "appitem.h"
#include "datasource.h"

#include <KDE/KFileItemList>

#include <QtCore/QBasicTimer>

class SizesCalculator;
namespace KIO {
    class PreviewJob;
}
class KJob;

class DataSource_Documents : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Documents(QObject *parent, SizesCalculator *inConstants);
    Q_INVOKABLE virtual int getItemCount() const;
    Q_INVOKABLE virtual QVariantMap getContent(int index) const;

    Q_INVOKABLE QString itemUrlDnd(int id) const;
    QPixmap getPreview(QString destination) const;
    Q_INVOKABLE void clearClicked();

signals:
    void newItemData(QVariantMap itemData, QString group);
    void runDesktopFile(QString desktopFile);
    void updateItemData(int id, QString field, QVariant data);

public slots:
    virtual void itemClicked(int newIndex);
    void updateContent();
    void updateThumbnails();
    void iconSizeChanged();

private slots:
    void resultPreviewJob(const KFileItem &item, const QPixmap &pixmap);
    void previewFailed(const KFileItem &item);
    void previewJobFinished(KJob *job);
    void onRecentsChanged(QString name);

private:
    virtual void onUpdateAllowedChanged();
    virtual void timerEvent(QTimerEvent *);
    void createDocumentsPreviews(KFileItemList list);

    AppItemList docsList;
    QStringList m_previewJobPlugins;
    SizesCalculator *constants;
    QMap<QString, QPixmap> m_pixmaps;
    KIO::PreviewJob* runningJob;
    QBasicTimer m_updateDelayTimer; // delay update to skip numerous notifications about changes
    bool m_recentsChanged; // indicates that recent documents is changed
};
