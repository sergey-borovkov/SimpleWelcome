#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Favorites : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Favorites(QObject* parent = 0);
    Q_INVOKABLE virtual int getItemCount(QString group = "") { return favoritesList.count(); }

signals:
    void newItemData(QString iconPath, QString name, int id);
    void dataClear();

public slots:
    virtual void itemClicked(int newIndex);
    virtual void updateContent();

private:
    QList<AppItem> favoritesList;
};
