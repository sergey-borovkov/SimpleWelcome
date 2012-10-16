#pragma once

#include <QString>
#include <QDebug>
#include <QtCore/QStringList>
#include <QApplication>
#include <QHash>
#include <QVariantMap>

class AppItem : public QVariantMap
{
public:
    bool operator <(const AppItem &other) const {
        return value("caption").toString().compare(other.value("caption").toString(), Qt::CaseInsensitive) < 0;
    }
};

/* Item in QML is a dictionary with the following fields:
 * -----
 * imagePath - path to image provider to get icon
 * caption - the one that's displayed under icon
 * id - uniqie identifier, used to interact between C++ and QML
 * pinned - reserved for use in Recent Documents pinning
 * group - used in Apps to pass the group app belong to
 * stack - items stacked in this item, array of dictionaries // auto set to undefined
*/

inline uint qHash (const AppItem &item) {
    return qHash(item["caption"].toString());
}

class AppItemList : public QList<AppItem> { };
