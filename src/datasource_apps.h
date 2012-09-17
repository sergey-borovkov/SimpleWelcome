#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps;

class DataSource_Apps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Apps(QObject* parent = 0, DataSource_RecentApps *recentApps = 0);
    ~DataSource_Apps();
    Q_INVOKABLE virtual int getItemCount();

signals:
    void newItemData(QString iconPath, QString name, int id);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);
    virtual void getContent();
    void itemDragged(int fromIndex, int toIndex);

private:
    void saveData();

    AppItemList appsList;
    QString currentGroup;
    QString prevCurrentGroup;

    DataSource_RecentApps *recentApps;
    QList<QPair<QString, int> > userAppsOrder;

    void updateItems();
};
