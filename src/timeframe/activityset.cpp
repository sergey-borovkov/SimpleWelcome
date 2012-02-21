#include "activityset.h"

ActivitySet::ActivitySet(QObject *parent) :
    QObject(parent)
{
}

ActivitySet::~ActivitySet()
{
    qDeleteAll(activities);
}

void ActivitySet::addActivity(const QString &url, const QString &type)
{
    activities.append(new Activity(url, type));
}

int ActivitySet::count()
{
    return activities.count();
}

QDate ActivitySet::beginDate()
{
    return QDate();
}

QDate ActivitySet::endDate()
{
    return QDate();
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
}

QString Activity::getType()
{
    return type;
}

void Activity::setType(QString newType)
{
    type = newType;
}

