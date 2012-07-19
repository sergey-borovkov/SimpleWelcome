#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_RecentApps(QObject* parent = 0);
    ~DataSource_RecentApps();

signals:
    void newItemData(QString iconPath, QString name);
    void dataClear();

public slots:
    virtual void itemClicked(int newIndex);
    virtual void updateContent();

private:
    QList<AppItem> recentAppsList;
};
