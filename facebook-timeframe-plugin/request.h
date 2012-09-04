#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

class QNetworkReply;

class Request : public QObject
{
    Q_OBJECT
public:
    enum RequestType {
        Get,
        Post
    };

    explicit Request(RequestType type, QObject *parent = 0);
    void setUrl(const QUrl &url);
    void setMessage(const QString &message);
    void startQuery();

signals:
    void replyReady(QByteArray);
    void success();

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    RequestType m_requestType;
    QUrl m_url;
    QString m_message;
};

#endif // REQUEST_H
