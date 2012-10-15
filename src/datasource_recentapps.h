#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_RecentApps(QObject* parent = 0);
    ~DataSource_RecentApps();
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE QString itemUrlDnd(int id);
    void addRecentApp(QString desktopFile);

signals:
    void newItemData(QVariantMap itemData, QString group) const;
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
