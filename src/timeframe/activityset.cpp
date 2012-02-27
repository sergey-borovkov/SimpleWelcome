#include "activityset.h"

ActivitySet::ActivitySet(QObject *parent) :
    QObject(parent)
{

}

ActivitySet::~ActivitySet()
{
    qDeleteAll(activities);
}

void ActivitySet::addActivity(const QString &url, const QString &type, const QDate &date)
{
    activities.append(new Activity(url, type, date));

    this->date = date;

    emit countChanged();
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

QDate ActivitySet::getDate()
{
    return date;
}

Activity *ActivitySet::activity(int index)
{
    return activities.at(index);
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


