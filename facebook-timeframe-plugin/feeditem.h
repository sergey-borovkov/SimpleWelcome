#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <socialitem.h>
#include <QtCore/QMap>

class FeedItem : public SocialItem
{
public:
    virtual QString pluginName() const;
    virtual Type type() const;
    virtual QString id() const;
    virtual QVariant data(int role) const;

    void fillFromMap(QVariantMap map);

private:
    QString m_id;
    QMap<int, QString> m_data;
};

#endif // FEEDITEM_H
