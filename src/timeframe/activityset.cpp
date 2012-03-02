#include "activityset.h"

ActivitySet::ActivitySet(QObject *parent) :
    QObject(parent)
{

}

ActivitySet::ActivitySet(QList<Activity *> activities, QObject *parent) :
    QObject(parent), activities(activities)
{
}

ActivitySet::~ActivitySet()
{
    qDeleteAll(activities);
}

void ActivitySet::addActivity(const QString &url, const QString &type, const QDate &date)
{
    addActivity(new Activity(url, type, date));
}

void ActivitySet::addActivity(Activity *activity)
{
    date = activity->getDate();
    activities.append(activity);
    emit countChanged();
}

int ActivitySet::count()
{
    return activities.count();
}

Activity *ActivitySet::activity(int index)
{
    return activities.at(index);
}

QString ActivitySet::getUrl(int i)
{
    if(i >= activities.length())
        return "";
    return activities.at(i)->getUrl();
}

QString ActivitySet::getType(int i)
{
    if(i >= activities.length())
        return "";
    else
        return activities.at(i)->getType();
}

QString Activity::getUrl()
{
    return url;
}

void Activity::setUrl(QString newUrl)
{
    url = newUrl;
    emit urlChanged();
}

QString Activity::getType()
{
    return type;
}

QDate ActivitySet::getDate(int i)
{
    if(i >= activities.length())
        return QDate();
    else
        return activities.at(i)->getDate();
}



void Activity::setType(QString newType)
{
    type = newType;
    emit typeChanged();
}

QDate Activity::getDate()
{
    return date;
}

void Activity::setDate(QDate date)
{
    this->date = date;
}


