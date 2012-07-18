#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

class Request : public QObject
{
    Q_OBJECT
public:
    enum RequestType
    {
        WallPosts,
        User,
        Logout
    };

    explicit Request(const QString &accessToken, RequestType type, QObject *parent = 0);
    void startQuery();

signals:
    void replyReady(QByteArray);

private slots:
    void replyFinished(QNetworkReply *reply);
    void networkError(QNetworkReply::NetworkError);

private:
    static const QString wallUrl;
    static const QString logoutUrl;
    RequestType m_type;
    QString m_accessToken;
};

#endif // REQUEST_H
