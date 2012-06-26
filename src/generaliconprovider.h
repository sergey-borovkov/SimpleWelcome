#ifndef __generaliconprovider_h_
#define __generaliconprovider_h_

#include <QDeclarativeImageProvider>

class RecentAppsProvider;
class PlacesProvider;
class DocumentsProvider;
class UserInfoProvider;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider(void);
    ~GeneralIconProvider(void);

    void setIsLocal(bool isLocal) { m_isLocal = isLocal; }
    void setRecentAppsProvider(RecentAppsProvider *recentAppsProvider) { m_recentAppsProvider = recentAppsProvider; }
    void setPlacesProvider(PlacesProvider *placesProvider) { m_placesProvider = placesProvider; }
    void setDocumentsProvider(DocumentsProvider *documentsProvider) { m_documentsProvider = documentsProvider; }
    void setUserInfoProvider(UserInfoProvider *userInfoProvider) { m_userInfoProvider = userInfoProvider; }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    bool m_isLocal;
    RecentAppsProvider *m_recentAppsProvider;
    PlacesProvider *m_placesProvider;
    DocumentsProvider *m_documentsProvider;
    UserInfoProvider *m_userInfoProvider;
};

#endif // __generaliconprovider_h_
