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

    if(m_beginDate.isValid())
    {
        if(m_beginDate > date)
            m_beginDate = date;
        else if(m_endDate < date)
            m_endDate = date;
    }
    else
    {
        m_beginDate = date;
        m_endDate = date;
    }


}

int ActivitySet::count()
{
    return activities.count();
}

QDate ActivitySet::beginDate()
{
    return m_beginDate;
}

QDate ActivitySet::endDate()
{
    return m_endDate;
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

QDate Activity::getDate()
{
    return date;
}

void Activity::setDate(QDate date)
{
    this->date = date;
}


