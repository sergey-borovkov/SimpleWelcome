#include "activity.h"

QString Activity::url() const
{
    return m_url;
}

void Activity::setUrl(const QString & url)
{
    m_url = url;
}

QString Activity::type() const
{
    return m_type;
}

void Activity::setType(const QString & type)
{
    m_type = type;
}

QDate Activity::date() const
{
    return m_date;
}

void Activity::setDate(const QDate & date)
{
    m_date = date;
}

QSize Activity::imageSize() const
{
    return m_imageSize;
}

void Activity::setImageSize(const QSize &imageSize)
{
    m_imageSize = imageSize;
}

QString Activity::mimetype() const
{
    return m_mimetype;
}

void Activity::setMimetype(const QString &mimetype)
{
    m_mimetype = mimetype;
}
