#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Apps : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Apps(QObject* parent = 0);
    Q_INVOKABLE virtual int getItemCount() { return appsList.count(); }

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
