#pragma once
#include <KUniqueApplication>
#include <KShortcut>
#include <QTimer>

#include <QtCore/QString>

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
    void globalActionTriggered();

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
};

class SizesCalculator : public QObject
{
    Q_OBJECT

public:
    SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer);

    Q_PROPERTY(int textToGridSpacing READ textToGridSpacing CONSTANT) int textToGridSpacing() const {
        return 16;
    }
    Q_PROPERTY(int groupTextHeight READ groupTextHeight CONSTANT) int groupTextHeight() const {
        return 24;
    }
    Q_PROPERTY(int gridWithGroupsSpacing READ gridWithGroupsSpacing CONSTANT) int gridWithGroupsSpacing() const {
        return 32;
    }
    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT) int gridColumns() const {
        return 7;
    }
    Q_PROPERTY(int cellWidth READ cellWidth NOTIFY iconSizeChanged) int cellWidth() const;
    Q_PROPERTY(int cellHeight READ cellHeight NOTIFY iconSizeChanged) int cellHeight() const;
    Q_PROPERTY(int iconTextSize READ iconTextSize NOTIFY iconSizeChanged) int iconTextSize() const;
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconSizeChanged) int iconSize() const;

    int availableHeight() const;

signals:
    void iconSizeChanged();

private:
    QmlApplicationViewer *viewer;
};
