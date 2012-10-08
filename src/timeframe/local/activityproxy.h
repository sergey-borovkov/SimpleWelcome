#ifndef GALLERYLISTER_H
#define GALLERYLISTER_H

#include "nepomuksource.h"

#include <QtCore/QDate>
#include <QtCore/QObject>

class Activity;
class LocalDayModel;

class ActivityProxy : public QObject
{
    Q_OBJECT
public:
    explicit ActivityProxy(QObject *parent = 0);

    void addNepomukSource(NepomukSource* source);
    void setModel(LocalDayModel* model);

signals:
    void newActivities(QList<Activity*>);
    void newMonth(int, int , QString type);
    void finished();
    void search();

public slots:
    void startSearch();
    int getIndexByDate(int year, int month,  bool direction);
    QDate getDateOfIndex(int listIndex);

private slots:
    void newData(QList<Activity*>);

private:
    LocalDayModel* m_model;
    NepomukSource* m_source;
};

#endif // GALLERYLISTER_H
