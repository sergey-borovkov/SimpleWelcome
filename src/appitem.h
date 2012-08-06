#pragma once

#include <QString>

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;

    bool operator <(const AppItem &other) const
    {
        return caption.compare(other.caption, Qt::CaseInsensitive) < 0;
    }
};
