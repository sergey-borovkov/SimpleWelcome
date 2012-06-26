#include "requestmanager.h"
#include "oauth2authorizer.h"

RequestManager::RequestManager(QObject *parent)
    : m_authorizer(0)
{
}

void RequestManager::queryWall(const QDate &beginDate, const QDate &endDate)
{
    if(!m_authorizer)
    {
        const QString token = m_authorizer->accessToken();
        //"https://graph.facebook.com/me/feed?access_token=AAAAAAITEghMBAH7SVnDZCXBF4OR9deZCtT1Crtj83mkiiwgjknlrQaBWsJ8kPUR9d3kBdQjTdtKeyyzPZCnhwgStKmki5vlh3wmiZBdwnw5FImZBULs5l"
        const QString request = QLatin1String("https://graph.facebook.com/me/feed?access_token=") + m_authorizer->accessToken();
    }
}

void RequestManager::setAuthorizer(OAuth2Authorizer *authorizer)
{
    m_authorizer = authorizer;
}
