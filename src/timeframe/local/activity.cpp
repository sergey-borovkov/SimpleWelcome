#include "activity.h"

QString Activity::url() const
{
    return m_url;
}

void Activity::setUrl(QString url)
{
    m_url = url;
}

QString Activity::type() const
{
    return m_type;
}

void Activity::setType(QString type)
{
    m_type = type;
}

QDate Activity::date() const
{
    return m_date;
}

void Activity::setDate(QDate date)
{
    m_date = date;
}

QString Activity::mimetype() const
{
    return m_mimetype;
}

void Activity::setMimetype(QString mimetype)
{
    m_mimetype = mimetype;
}
