#pragma once

#include <QString>
#include <QDebug>
#include <QtCore/QStringList>
#include <QApplication>
#include <QHash>
#include <QVariantMap>

/* Item in QML is a dictionary with the following fields:
 * -----
 * imagePath - path to image provider to get icon
 * caption - the one that's displayed under icon
 * id - uniqie identifier, used to interact between C++ and QML
 * pinned - reserved for use in Recent Documents pinning
 * group - used in Apps to pass the group app belong to
 * stack - items stacked in this item, array of dictionaries // auto set to undefined
*/

typedef QVariantMap AppItem;
typedef QList<AppItem> AppItemList;

inline uint qHash(const AppItem &item)
{
    return qHash(item["caption"].toString());
}
