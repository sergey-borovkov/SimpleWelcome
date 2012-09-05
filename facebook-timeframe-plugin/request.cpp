#include "request.h"

#include <QtNetwork/QNetworkAccessManager>

FacebookRequest::FacebookRequest(RequestType type, QObject *parent) :
    QObject(parent), m_requestType(type)
{
}

void FacebookRequest::setUrl(const QUrl &url)
{
    m_url = url;
}

void FacebookRequest::start()
{
    if(m_url.isEmpty())
        return;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));

    QNetworkReply *reply = 0;
    if(m_requestType == Get)
        reply = manager->get(QNetworkRequest(m_url));
    else if(m_requestType == Post)
        reply = manager->post(QNetworkRequest(m_url), m_message.toUtf8());

    if(reply != 0) {
        connect(reply, SIGNAL(finished()), SIGNAL(success()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    }
}

void FacebookRequest::replyFinished(QNetworkReply *reply)
{
    QByteArray a = reply->readAll();
    emit replyReady(a);
    reply->deleteLater();
}

void FacebookRequest::error(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error)
    const QNetworkReply *reply = qobject_cast<const QNetworkReply *>(sender());
    emit reply->errorString();
}
