#pragma once

#include <QString>
#include <QDebug>
#include <QStringList>
#include <QApplication>

class AppItem
{
public:
    QString caption;
    QString icon;
    QString desktopEntry;
    QString relPath;

    bool operator <(const AppItem &other) const {
        return caption.compare(other.caption, Qt::CaseInsensitive) < 0;
    }

};

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

