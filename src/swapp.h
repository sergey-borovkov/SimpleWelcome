#ifndef SW_APP_H
#define SW_APP_H

#include <KUniqueApplication>

#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "appprovider.h"
#include "appentity.h"
#include "appiconprovider.h"
#include "searchrunner.h"
#include "generaliconprovider.h"
#include "recentappsprovider.h"
#include "placesprovider.h"
#include "documentsprovider.h"
#include "sessionprovider.h"
#include "userinfoprovider.h"

class ActivityProxy;
class NepomukSource;
class QThread;

class SWApp : public KUniqueApplication
{
    Q_OBJECT
public:
    SWApp();
    ~SWApp();

    int newInstance();

    static SWApp* self();

public Q_SLOTS:
    bool event(QEvent *event);
    bool isLocal(void);

private Q_SLOTS:
    void init(void);
    
 private:
    bool m_inited;
    QmlApplicationViewer *m_viewer;

    AppProvider *m_appProvider;
    AppIconProvider *m_appIconProvider;
    
      ActivityProxy* m_model;
    NepomukSource* m_source;

    QThread *m_nepomukThread;

    SearchRunner *m_searchRunner;
    GeneralIconProvider *m_generalIconProvider;
    RecentAppsProvider *m_recentAppsProvider;
    PlacesProvider *m_placesProvider;
    DocumentsProvider *m_documentsProvider;
    SessionProvider *m_sessionProvider;
    UserInfoProvider *m_userInfoProvider;
};

#endif // SW_APP_H
