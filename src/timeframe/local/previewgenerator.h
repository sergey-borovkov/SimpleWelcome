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

#include <QDeclarativeEngine>
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QSet>

#include <kio/previewjob.h>
#include <kfileitem.h>

class FileModel;
class LocalDayModel;

class PreviewGenerator : public QObject
{
    Q_OBJECT

public:
    static PreviewGenerator *instance();
    QPixmap getPreviewPixmap(QString filePath);
    void setModel(LocalDayModel* model);

public slots:
    void start(const QStringList& list);


private slots:
    void setPreview(const KFileItem&, const QPixmap&);
    void setNullIcon(const KFileItem &item);
    void notifyModel(const QString& filePath);

private:
    explicit PreviewGenerator(QObject *parent = 0);
    LocalDayModel * m_model;

    QHash<QString, QPixmap> m_previews;
    QSet<QString> m_files;
    QPixmap defaultPreview;

    static PreviewGenerator *m_instance;
    QPixmap videoPixmap;

    KIO::PreviewJob *m_job;
    QStringList m_plugins;
    KFileItemList m_fileList;
};

#endif // PREVIEWGENERATOR_H
