#pragma once
#include <KUniqueApplication>

#include <QtCore/QString>

class QmlApplicationViewer;
class GeneralIconProvider;

class NepomukSource;
class SocialProxy;
class QThread;
class ActivityProxy;

class SWApp : public KUniqueApplication
{
    Q_OBJECT

public:
    SWApp();
    ~SWApp();

    static SWApp* self();
    static QString pathToShareDir(); ///< Returns path to 'share' directory

public Q_SLOTS:
    bool event(QEvent *event);

private:
    void initTimeframeLocalMode();
    void initTimeframeSocialMode();

    QmlApplicationViewer *m_viewer;

    NepomukSource       *m_source;
    QThread             *m_nepomukThread;
    ActivityProxy       *m_proxy;
    SocialProxy         *m_manager;

    GeneralIconProvider *m_generalIconProvider;
};

class QMLConstants : public QObject
{
    Q_OBJECT

public:
    QMLConstants(QObject *parent, QmlApplicationViewer *inViewer);

    Q_PROPERTY(int textToGridSpacing READ textToGridSpacing CONSTANT) int textToGridSpacing() { return 16; }
    Q_PROPERTY(int groupTextHeight READ groupTextHeight CONSTANT) int groupTextHeight() { return 24; }
    Q_PROPERTY(int gridWithGroupsSpacing READ gridWithGroupsSpacing CONSTANT) int gridWithGroupsSpacing() { return 32; }
    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT) int gridColumns() { return 7; }
    Q_PROPERTY(int cellWidth READ cellWidth NOTIFY iconSizeChanged) int cellWidth();
    Q_PROPERTY(int cellHeight READ cellHeight NOTIFY iconSizeChanged) int cellHeight();
    Q_PROPERTY(int iconTextSize READ iconTextSize NOTIFY iconSizeChanged) int iconTextSize();
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconSizeChanged) int iconSize();

signals:
    void iconSizeChanged();

private:
    QmlApplicationViewer *viewer;
};
