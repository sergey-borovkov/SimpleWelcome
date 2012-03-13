#ifndef SW_APP_H
#define SW_APP_H

#include <KApplication>

#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "appprovider.h"
#include "appentity.h"
#include "appiconprovider.h"


class ActivityProxy;
class NepomukSource;
class QThread;

class SWApp : public KApplication
{
    Q_OBJECT
public:
    SWApp();
    ~SWApp();

    int newInstance();

    static SWApp* self();

public Q_SLOTS:
    bool event(QEvent *event);

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
};

#endif // SW_APP_H
