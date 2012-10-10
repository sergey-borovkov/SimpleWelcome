#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QMetaType>

class Activity
{
public:
    explicit Activity(const QString &url, const QString &type, const QDate &date, const QString &mimetype)
        : m_mimetype(mimetype), m_url(url), m_type(type), m_date(date) {}

    QString mimetype() const;
    void setMimetype(QString mimetype);

    QString url() const;
    void setUrl(QString url);

    QString type() const;
    void setType(QString type);

    QDate date() const;
    void setDate(QDate date);

private:
    QString m_mimetype;
    QString m_url;
    QString m_type;
    QDate m_date;
};

Q_DECLARE_METATYPE(Activity *)

#endif // ACTIVITY
