/* Timeframe
 * Copyright (c) 2012 ROSA  <support@rosalab.ru>
 * Authors:
 *  Sergey Borovkov <sergey.borovkov@osinit.ru>
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

#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include <socialplugin.h>

class QNetworkReply;
class QNetworkAccessManager;

class FacebookRequest : public QObject, public Request
{
    Q_OBJECT

public:
    enum RequestType {
        Get,
        Post,
        Delete
    };

    explicit FacebookRequest(RequestType type, QObject *parent = 0);
    void setUrl(const QUrl &url);

signals:
    void replyReady(QByteArray);
    void success();
    void newItemId(QString);
    void gotUserPictureUrl(QString, QString);
    void error(QString);

private slots:
    void replyFinished();
    void postFinished();
    void error(QNetworkReply::NetworkError error);

protected:
    void run();

private:
    RequestType m_requestType;
    QUrl m_url;
    QString m_message;
    static QNetworkAccessManager *manager;
};

#endif // REQUEST_H
