#include "request.h"

#include <QtNetwork/QNetworkAccessManager>

QNetworkAccessManager *FacebookRequest::manager = 0;

FacebookRequest::FacebookRequest(RequestType type, QObject *parent) :
    QObject(parent), m_requestType(type)
{
    // this is never deleted
    if(!manager)
        manager = new QNetworkAccessManager();
}

void FacebookRequest::setUrl(const QUrl &url)
{
    m_url = url;
}

void FacebookRequest::start()
{
    if(m_url.isEmpty())
        return;

    QNetworkReply *reply = 0;
    QNetworkRequest request(m_url);

    switch(m_requestType)
    {
    case Get:
        reply = manager->get(request);
        break;
    case Post:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        reply = manager->post(request, QByteArray());
        break;
    default:
        qWarning("FacebookRequest::start() -- Invalid argument");
        return;
    }

    connect(reply, SIGNAL(finished()), SLOT(replyFinished()));
    connect(reply, SIGNAL(finished()), SIGNAL(success()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
}

void FacebookRequest::replyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray answer = reply->readAll();
    emit replyReady(answer);
    reply->deleteLater();
}

void FacebookRequest::error(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    emit reply->errorString();
    reply->deleteLater();
}
