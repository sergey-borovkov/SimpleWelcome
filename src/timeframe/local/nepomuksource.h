#ifndef NEPOMUKSOURCE_H
#define NEPOMUKSOURCE_H

#include <QDate>
#include <QTimer>

class Activity;

class NepomukSource : public QObject
{
    Q_OBJECT
public:
    explicit NepomukSource(QObject *parent = 0);

public slots:
    virtual void startSearch();

signals:
    void searchFinished();
    void newActivities(QList<Activity *>);
};

#endif // NEPOMUKSOURCE_H
