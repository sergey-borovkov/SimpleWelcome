#ifndef OAUTH2AUTHORIZER_H
#define OAUTH2AUTHORIZER_H

#include <QtCore/QObject>

class QUrl;

class OAuth2Authorizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
public:
    static const QString redirectUrl;

    explicit OAuth2Authorizer(QObject *parent = 0);
    QString accessToken() const;
    void setAccessToken(const QString &accessToken);

    bool isAuthenticated() const;

signals:
    void accessTokenChanged(QString accessToken);

public slots:
    void urlChanged(const QUrl &url);

private:
    QString m_accessToken;
};

#endif // OAUTH2AUTHORIZER_H
