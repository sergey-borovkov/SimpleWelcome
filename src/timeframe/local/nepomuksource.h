#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include <QtCore/QObject>

class Activity;

class NepomukSource : public QObject
{
    Q_OBJECT
public:
    explicit NepomukSource(QObject *parent = 0);

public slots:
    virtual void startSearch();

    bool isNepomukInitialized();
    void nepomukConfigure();

signals:
    void searchFinished();
    void newActivities(QList<Activity *>);
};

#endif // NEPOMUKSOURCE_H
