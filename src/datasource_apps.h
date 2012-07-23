#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Apps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Apps(QObject* parent = 0)
        : DataSource(parent), prevCurrentGroup("-1") {}

signals:
    void newItemData(QString iconPath, QString name, int id);
    void dataClear();

public slots:
    virtual void itemClicked(int newIndex);
    virtual void updateContent();

private:
    QList<AppItem> appsList;
    QString currentGroup;
    QString prevCurrentGroup;
};
