#pragma once

#include <QString>
#include <QDebug>
#include <QtCore/QStringList>
#include <QApplication>
#include <QHash>

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;
    QString destination;

    bool operator <(const AppItem &other) const {
        return caption.compare(other.caption, Qt::CaseInsensitive) < 0;
    }

    bool operator ==(AppItem &other) const {
        return other.caption == caption && other.desktopEntry == desktopEntry && other.icon == icon && other.relPath == relPath && other.destination == destination;
    }
};

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
