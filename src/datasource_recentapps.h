#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_RecentApps(QObject *parent = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);
    void addRecentApp(QString desktopFile, bool isPinned = false);

signals:
    void newItemData(QVariantMap itemData, QString group) const;
    void resetContent();
    void runDesktopFile(QString desktopFile);
    void updateItemData(int id, QString field, QVariant data);

public slots:
    virtual void itemClicked(int newIndex);
    void itemPinnedToggle(int index);
    void itemDragged(int fromIndex, int toIndex);

private:
    void saveData();

    AppItemList recentAppsList;
};
