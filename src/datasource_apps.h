#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps;

class DataSource_Apps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Apps(QObject* parent = 0, DataSource_RecentApps *recentApps = 0);
    Q_INVOKABLE virtual int getItemCount();

    Q_INVOKABLE QString itemUrlDnd(int id);

signals:
    void newItemData(QVariantMap itemData);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);
    virtual void getContent();
    void updateItems(bool isResetContent = true);

private:
    void saveData();

    AppItemList appsList;
    QString currentGroup;
    QString prevCurrentGroup;

    DataSource_RecentApps *recentApps;
    QList<QPair<QString, int> > userAppsOrder;
};
