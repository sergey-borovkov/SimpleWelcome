#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_RecentApps(QObject* parent = 0);
    ~DataSource_RecentApps();
    Q_INVOKABLE virtual int getItemCount();
    void addRecentApp(QString desktopFile);

signals:
    void newItemData(QString iconPath, QString name, int id, bool isPinned) const;
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);
    virtual void getContent();
    void itemDragged(int fromIndex, int toIndex);

private:
    void saveData();

    QList<AppItem> recentAppsList;
};
