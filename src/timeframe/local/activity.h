#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QMetaType>

class Activity
{
public:
    explicit Activity(const QString &url, const QString &type, const QDate &date, const QString &mimetype, const QSize &imageSize)
        : m_mimetype(mimetype), m_url(url), m_type(type), m_date(date), m_imageSize(imageSize) {}

    QString mimetype() const;
    void setMimetype(const QString &mimetype);

    QString url() const;
    void setUrl(const QString &url);

    QString type() const;
    void setType(const QString &type);

    QDate date() const;
    void setDate(const QDate &date);

    QSize imageSize() const;
    void setImageSize(const QSize &imageSize);

private:
    QString m_mimetype;
    QString m_url;
    QString m_type;
    QDate m_date;
    QSize m_imageSize;
};

Q_DECLARE_METATYPE(Activity *)

#endif // ACTIVITY
