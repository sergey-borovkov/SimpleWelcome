#include "activityset.h"

ActivitySet::ActivitySet(QObject *parent) :
    QObject(parent)
{
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

