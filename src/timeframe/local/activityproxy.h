#ifndef GALLERYLISTER_H
#define GALLERYLISTER_H

#include "nepomuksource.h"

#include <QtCore/QObject>
#include <QtCore/QDate>

class Activity;
class LocalContentModel;

class ActivityProxy : public QObject
{
    Q_OBJECT
public:
    explicit ActivityProxy(NepomukSource *source, QObject *parent = 0);
    void setModel(LocalContentModel *model);

signals:
    void newActivities(QList<Activity *>);
    void newMonth(int, int , QString type);
    void searchFinished();
    void search();

public slots:
    void startSearch();

private slots:
    void newData(QList<Activity *>);

private:
    LocalContentModel *m_model;
};

#endif // GALLERYLISTER_H
