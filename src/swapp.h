#ifndef SW_APP_H
#define SW_APP_H

#include <KUniqueApplication>

#include "qmlapplicationviewer/qmlapplicationviewer.h"

#include "generaliconprovider.h"
#include "sessionprovider.h"
#include "userinfoprovider.h"

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

    GeneralIconProvider *m_generalIconProvider;
};

#endif // SW_APP_H
