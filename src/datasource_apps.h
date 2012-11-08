#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps;

class DataSource_Apps : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Apps(QObject *parent = 0, DataSource_RecentApps *recentApps = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);
    void updateItems(bool isResetContent = true);

private:
    void saveData();

    AppItemList appsList;
    QString currentGroup;
    QString prevCurrentGroup;

    DataSource_RecentApps *recentApps;
};
