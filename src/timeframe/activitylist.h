#ifndef ACTIVITYLIST_H
#define ACTIVITYLIST_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QMetaType>

class ActivitySet;

class ActivityList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QDate date READ date NOTIFY dateChanged)
public:
    explicit ActivityList(int year, int month, QObject *parent = 0);
    ActivityList() {}

    void setDate(int y, int m) { d.setDate(y, m, 1); emit dateChanged();}

signals:    
    void countChanged();
    void dateChanged();

public slots:
    int count();
    QDate date() const { return d; }
    void addSet(ActivitySet *set);
    ActivitySet *at(int i) const;

    QString getUrl(int set, int index);
    int getSetCount(int index);
    QDate getSetDate(int index);

private:
    QList < ActivitySet* > list;
    QDate d;
};

Q_DECLARE_METATYPE(ActivityList *)

#endif // ACTIVITYLIST_H
