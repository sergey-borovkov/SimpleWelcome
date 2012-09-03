#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

#include <socialplugin.h>

class VkRequest : public QObject, public Request
{
    Q_OBJECT
public:
    enum RequestType {
        WallPosts,
        User,
        Logout
    };

    explicit VkRequest(const QString &accessToken, RequestType type, QObject *parent = 0, int offset = 0);
    void start();

signals:
    void replyReady(QByteArray);
    void success();

private slots:
    void replyFinished(QNetworkReply *reply);
    void networkError(QNetworkReply::NetworkError);

private:
    static const QString wallUrl;
    static const QString logoutUrl;
    RequestType m_type;
    QString m_accessToken;
    int m_offset;
};

#endif // REQUEST_H
