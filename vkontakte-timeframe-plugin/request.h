#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include <socialplugin.h>

class VkRequest : public QObject, public Request
{
    Q_OBJECT
public:
    enum RequestType {
        Get,
        Post,
        Delete
    };

    explicit VkRequest(RequestType type, QObject *parent = 0);
    void setUrl(const QUrl &url);
    void start();

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

private:
    RequestType m_requestType;
    QUrl m_url;
    QString m_message;
    static QNetworkAccessManager *manager;
};

#endif // REQUEST_H
