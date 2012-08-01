#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Documents : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Documents(QObject* parent = 0);
    Q_INVOKABLE virtual int getItemCount() { return docsList.count(); }

signals:
    void newItemData(QString iconPath, QString name, int id);
    void dataClear();

public slots:
    virtual void itemClicked(int newIndex);
    virtual void updateContent();

private:
    QList<AppItem> docsList;
};
