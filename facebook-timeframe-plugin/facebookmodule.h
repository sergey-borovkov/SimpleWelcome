#ifndef FACEBOOK_MODULE_H
#define FACEBOOK_MODULE_H

#include <QtGui/QPixmap>
#include <socialplugin.h>
#include "oauth2authorizer.h"

class QWebView;
class RequestManager;
class OAuth2Authorizer;

class FacebookModule : public QObject, public ISocialPlugin
{
    Q_OBJECT
    Q_INTERFACES(ISocialPlugin)
public:
    FacebookModule();
    ~FacebookModule();

    virtual ISocialRequestManager* requestManager();
    QString name() const;
    QPixmap icon() const;
    QPixmap smallIcon() const;

    QWidget *authenticationWidget();
    bool authorized() const {
        return !m_authorizer->accessToken().isEmpty();
    }

    QString selfId() const;
    void setSelfId( const QString& id );

    QString selfName() const;
    void setSelfName( const QString & name );

    QString selfPictureUrl() const;
    void setSelfPictureUrl( const QString & url);

private slots:
    void onAcessTokenChanged();

signals:
    void authorized();
    void deauthorized();

private:
    QWebView *m_authorizationView;
    RequestManager *m_requestManager;
    OAuth2Authorizer *m_authorizer;
    QPixmap m_pixmap;
    QPixmap m_smallPixmap;

};

#endif
