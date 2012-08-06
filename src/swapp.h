#pragma once
#include <KUniqueApplication>

class QmlApplicationViewer;
class GeneralIconProvider;

class SWApp : public KUniqueApplication
{
    Q_OBJECT

public:
    SWApp();
    ~SWApp();

    static SWApp* self();

public Q_SLOTS:
    bool event(QEvent *event);
    bool isLocal();

private:
    QmlApplicationViewer *m_viewer;
    GeneralIconProvider *m_generalIconProvider;
};

class QMLConstants : public QObject
{
    Q_OBJECT

public:
    QMLConstants(QObject * parent) : QObject(parent) {}

    Q_PROPERTY(int textToGridSpacing READ textToGridSpacing CONSTANT) int textToGridSpacing() { return 16; }
    Q_PROPERTY(int groupTextHeight READ groupTextHeight CONSTANT) int groupTextHeight() { return 24; }
    Q_PROPERTY(int gridWithGroupsSpacing READ gridWithGroupsSpacing CONSTANT) int gridWithGroupsSpacing() { return 32; }
    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT) int gridColumns() { return 7; }
    Q_PROPERTY(int cellWidth READ cellWidth CONSTANT) int cellWidth() { return 140; } // 120
    Q_PROPERTY(int cellHeight READ cellHeight CONSTANT) int cellHeight() { return 200; }
};
