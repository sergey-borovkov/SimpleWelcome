#ifndef GALLERYLISTER_H
#define GALLERYLISTER_H

#include <QObject>
#include <QDate>

#include "nepomuksource.h"

class Activity;

class GalleryLister : public QObject
{
    Q_OBJECT
public:
    explicit GalleryLister(QObject *parent = 0);

    void addNepomukSource(NepomukSource* source);
signals:
    void newActivities (QList<Activity*>);
    void newMonth(int, int);
public slots:
    void startSearch(QDate date, int direction);

private:

    NepomukSource* m_source;
};

#endif // GALLERYLISTER_H
