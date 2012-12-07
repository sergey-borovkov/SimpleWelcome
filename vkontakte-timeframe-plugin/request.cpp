#include "request.h"

#include <QtNetwork/QNetworkAccessManager>
#include <qjson/parser.h>

QNetworkAccessManager *VkRequest::manager = 0;

VkRequest::VkRequest(RequestType type, QObject *parent)
    : QObject(parent)
    , m_requestType(type)
{
    // this is never deleted
    if (!manager)
        manager = new QNetworkAccessManager();
}

void VkRequest::setUrl(const QUrl &url)
{
    m_url = url;
}

void VkRequest::start()
{
    if (m_url.isEmpty())
        return;
    QNetworkReply *reply = 0;

    QNetworkRequest request(m_url);

    switch (m_requestType) {
    case Get:
        reply = manager->get(request);
        connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
        connect(reply, SIGNAL(finished()), SIGNAL(success()));
        break;
    case Post:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        reply = manager->post(request, QByteArray());
        connect(reply, SIGNAL(finished()), SLOT(postFinished()));
        break;
    case Delete:
        reply = manager->deleteResource(request);
        connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
        connect(reply, SIGNAL(finished()), SIGNAL(success()));
        break;

    default:
        qWarning("VkRequest::start() -- Invalid argument");
        return;
    }

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
}

void VkRequest::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray answer = reply->readAll();
    emit replyReady(answer);
    reply->deleteLater();
}

void VkRequest::postFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray answer = reply->readAll();
    QJson::Parser parser;

    QVariantMap result = parser.parse(answer).toMap();

    QString id;

    if (result.contains(QLatin1String("response"))) {
        QVariantMap map = result.value(QLatin1String("response")).toMap();

        if (map.contains(QLatin1String("post_id"))) {
            id = map.value(QLatin1String("post_id")).toString();
        }
    }

    if (!id.isEmpty())
        emit newItemId(id);

    emit success();
}

void VkRequest::error(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    emit reply->errorString();
    reply->deleteLater();
}
