/* Timeframe
 * Copyright (c) 2011-2012 ROSA  <support@rosalab.ru>
 * Authors: Julia Mineeva, Evgeniy Auzhin, Sergey Borovkov.
 * License: GPLv3
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

#ifndef PREVIEWGENERATOR_H
#define PREVIEWGENERATOR_H

#include "localdaymodel.h"

#include <QtCore/QHash>
#include <QtCore/QObject>

#include <KIO/PreviewJob>

class FileModel;
class LocalContentModel;
class LocalDayFilterModel;
struct PreviewItem;

class PreviewGenerator : public QObject
{
    Q_OBJECT
public:
    void setModel(LocalContentModel *model);

    QPixmap takePreviewPixmap(quintptr requestId);

    /**
     * @brief requests preview to be generated
     */
    Q_INVOKABLE void request(const QString &path, const QSize &size, quintptr id);

    /**
     * @brief cancel is called in QML when cloud component gets destroyed. It removes
     *        unused  images generated for model and cancels running job associated with path
     */
    Q_INVOKABLE void cancel(const QString &requestId);

private slots:
    void previewJobResult(const KFileItem &, const QPixmap &);
    void previewJobFailed(const KFileItem &item);
    void previewJobFinished(KJob *job);

private:
    friend PreviewGenerator *previewGenerator(const QString &type);

    explicit PreviewGenerator();
    void notifyModelAboutPreview(const QString &url);

    QHash<quintptr, QPixmap> m_previews;
    QHash<quintptr, KJob*> m_runningJobs;
    QPixmap m_videoPixmap;
    QStringList m_plugins;
    LocalContentModel *m_model;
};

/**
 * @brief previewGenerator returns instance of PreviewGenerator class
 * @param type is either "gallery" or "timeline"
 * @return
 */
PreviewGenerator *previewGenerator(const QString &type);


#endif // PREVIEWGENERATOR_H
