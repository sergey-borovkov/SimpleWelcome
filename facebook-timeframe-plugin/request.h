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
        Post
    };

    explicit FacebookRequest(RequestType type, QObject *parent = 0);
    void setUrl(const QUrl &url);
    void start();

signals:
    void replyReady(QByteArray);
    void success();
    void error(QString);
private slots:
    void replyFinished();
    void error(QNetworkReply::NetworkError error);

private:
    RequestType m_requestType;
    QUrl m_url;
    QString m_message;
    static QNetworkAccessManager *manager;
};

#endif // REQUEST_H
