#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Documents : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Documents(QObject* parent = 0)
        : DataSource(parent) {}

signals:
    void newItemData(QString iconPath, QString name);
    void dataClear();

public slots:
    virtual void itemClicked(int newIndex);
    virtual void updateContent();

private:
    QList<AppItem> docsList;
};
