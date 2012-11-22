#pragma once
#include <kdeclarative.h>
#include <KDE/KUniqueApplication>
#include <KDE/KShortcut>
#include <QtCore/QTimer>

class QmlApplicationViewer;
class GeneralIconProvider;

class NepomukSource;
class SocialProxy;
class QThread;
class ActivityProxy;

class KAction;

class SWApp : public KUniqueApplication
{
    Q_OBJECT

public:
    SWApp();
    ~SWApp();

    static SWApp *self();
    static QString pathToRoot(); ///< Returns path to root of installation


public Q_SLOTS:
    virtual int newInstance();
    bool event(QEvent *event);
    void runDesktopFile(QString desktopFile);

private:
    void loadShortcut();

    void initTimeframeLocalMode();
    void initTimeframeSocialMode();

    QmlApplicationViewer *m_viewer;

    NepomukSource       *m_source;
    QThread             *m_nepomukThread;
    ActivityProxy       *m_proxy;
    SocialProxy         *m_manager;

    GeneralIconProvider *m_generalIconProvider;
    KShortcut m_globalShortcut;

    QTimer m_periodicTimer;
    KAction *m_globalAction;
    KDeclarative m_kdeclarative;
};
