#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_Favorites : public DataSource
{
    Q_OBJECT

public:
    explicit DataSource_Favorites(QObject* parent = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE QString itemUrlDnd(int id);

signals:
    void newItemData(QVariantMap itemData);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);
    virtual void getContent();

private:
    QList<AppItem> favoritesList;
};
