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
