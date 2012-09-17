#pragma once

#include <QObject>

class SessionProvider : public QObject
{
    Q_OBJECT

public:
    SessionProvider(QObject *parent = 0)
        : QObject(parent) {}

public slots:
    void lock();
    void shutdown();
};
