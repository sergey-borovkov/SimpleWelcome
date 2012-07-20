#ifndef GALLERYLISTER_H
#define GALLERYLISTER_H

#include <QObject>
#include <QDate>

#include "nepomuksource.h"

class Activity;
class GalleryModel;

class GalleryLister : public QObject
{
    Q_OBJECT
public:
    explicit GalleryLister(QObject *parent = 0);

    void addNepomukSource(NepomukSource* source);
    void setModel(GalleryModel* model);

signals:
    void newActivities (QList<Activity*>);
    void newMonth(int, int);
    void changeFilterString(const QString&);

public slots:
    void startSearch(QDate date, int direction);
    void resultCount(int);
    int getIndexByDate(int year, int month,  bool direction);
    QDate getDateOfIndex(int listIndex);
    QObject* itemsModel(QDate date) const;
    void changeType(QString type);

private slots:
    void newData(QList<Activity*>);

private:
    GalleryModel* m_model;
    NepomukSource* m_source;
};

#endif // GALLERYLISTER_H
