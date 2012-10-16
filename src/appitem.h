#pragma once

#include <QString>
#include <QDebug>
#include <QtCore/QStringList>
#include <QApplication>
#include <QHash>
#include <QVariantMap>

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;
    QString destination;
    QString group;

    bool operator <(const AppItem &other) const {
        return caption.compare(other.caption, Qt::CaseInsensitive) < 0;
    }

    bool operator ==(AppItem &other) const {
        return other.caption == caption && other.desktopEntry == desktopEntry && other.icon == icon && other.relPath == relPath && other.destination == destination;
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
    return qHash(item.caption);
}


class AppItemList : public QList<AppItem>
{
public:
    QStringList toStringList() const
    {
        QStringList out;
        out.reserve(count());
        for (int i = 0; i < count(); i++)
            out.append(at(i).caption);
        return out;
    }
};
