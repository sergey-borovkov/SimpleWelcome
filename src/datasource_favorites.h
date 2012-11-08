#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Favorites : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Favorites(QObject *parent = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);

private:
    AppItemList favoritesList;
};
