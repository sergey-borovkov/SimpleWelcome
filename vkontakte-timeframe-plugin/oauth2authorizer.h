#ifndef OAUTH2AUTHORIZER_H
#define OAUTH2AUTHORIZER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

class OAuth2Authorizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
public:
    static const QString redirectUrl;

    explicit OAuth2Authorizer(QObject *parent = 0);
    QString accessToken() const;
    void setAccessToken(const QString &accessToken);
    bool isAuthorized() const;

signals:
    void accessTokenChanged(QString accessToken);
    void deauthorized();

public slots:
    void urlChanged(const QUrl &url);
    void logout();

private:
    QString m_accessToken;
};

#endif // OAUTH2AUTHORIZER_H
