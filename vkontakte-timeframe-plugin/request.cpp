#include "request.h"

#include <qjson/parser.h>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

const QString VkRequest::wallUrl = QLatin1String("https://api.vk.com/method/wall.get");
const QString VkRequest::logoutUrl = QLatin1String("http://oauth.vk.com/oauth/logout");
// response http://oauth.vk.com/oauth/authorize?&_hash=0&success=1

VkRequest::VkRequest(const QString &accessToken, RequestType type, QObject *parent, int offset)
    : QObject(parent)
    , m_accessToken(accessToken)
    , m_type(type)
    , m_offset(offset)
{
}

void VkRequest::start()
{
    QUrl requestUrl;

    if (m_type == WallPosts) {
        requestUrl = wallUrl;
        requestUrl.addQueryItem(QLatin1String("offset"), QString("%1").arg(m_offset));
        requestUrl.addQueryItem(QLatin1String("count"), "100");
        requestUrl.addQueryItem(QLatin1String("filter"), "all");

        requestUrl.addQueryItem(QLatin1String("access_token"), m_accessToken);
    } else if (m_type == Logout) {
        requestUrl = logoutUrl;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(requestUrl));
}

void VkRequest::replyFinished(QNetworkReply *reply)
{
    QByteArray a = reply->readAll();
    emit replyReady(a);
    reply->deleteLater();
}

void VkRequest::networkError(QNetworkReply::NetworkError)
{
}
