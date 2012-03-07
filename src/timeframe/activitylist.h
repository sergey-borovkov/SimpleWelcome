#ifndef ACTIVITYLIST_H
#define ACTIVITYLIST_H

#include <QObject>
#include <QList>
#include <QDate>

class ActivitySet;

class ActivityList : public QObject
{
    Q_OBJECT
public:
    explicit ActivityList(int year, int month, QObject *parent = 0);

    QDate date() const { return d; }
    void setDate(int y, int m) { d.setDate(y, m, 1); }

signals:    

public slots:
    int count();
    void addSet(ActivitySet *set);
    ActivitySet *at(int i) const;

private:
    QList < ActivitySet* > list;
    QDate d;
};

#endif // ACTIVITYLIST_H
