#include "request.h"

#include <qjson/parser.h>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>

const QString Request::wallUrl = "https://graph.facebook.com/me/feed";

Request::Request(const QString &accessToken, RequestType type, QObject *parent) :
    QObject(parent), m_accessToken(accessToken), m_type(type)
{
}

void Request::startQuery()
{
    qDebug() << "startQuery";
    QUrl requestUrl;

    if(m_type == WallPosts)
    {
        requestUrl = wallUrl;
        requestUrl.addQueryItem(QLatin1String("access_token"), m_accessToken);
    }
    qDebug() << "url" << requestUrl;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(networkError(QNetworkReply::NetworkError)));
    manager->get(QNetworkRequest(requestUrl));
}

void Request::replyFinished(QNetworkReply *reply)
{
    QByteArray a = reply->readAll();
    QJson::Parser parser;
    QVariantMap result = parser.parse(a).toMap();

    QVariantList list = result.value("data").toList();

    foreach(QVariant item, list)
    {
        QVariantMap map = item.toMap();
        QStringList keys = map.keys();

        qDebug() << "NEW ITEM";
        for(int i = 0; i < keys.size(); i++)
        {
            qDebug() << keys[i] << map.value(keys[i]);
        }
    }

    emit replyReady(a);
    reply->deleteLater();
}

void Request::networkError(QNetworkReply::NetworkError)
{
}
