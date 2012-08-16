#include "request.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

Request::Request(RequestType type, QObject *parent) :
    QObject(parent), m_requestType(type)
{
}

void Request::setUrl(const QUrl &url)
{
    m_url = url;
}

void Request::startQuery()
{
    if(m_url.isEmpty())
        return;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), SIGNAL(success()));

    if(m_requestType == Get)
        manager->get(QNetworkRequest(m_url));
    else if(m_requestType == Post)
        manager->post(QNetworkRequest(m_url), m_message.toUtf8());
}

void Request::replyFinished(QNetworkReply *reply)
{
    QByteArray a = reply->readAll();
    emit replyReady(a);
    reply->deleteLater();
}
