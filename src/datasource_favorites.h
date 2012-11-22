#pragma once

#include "appitem.h"
#include "datasource.h"

#include <QtCore/QModelIndex>
#include <QtCore/QBasicTimer>

// Forward declarations
class KFilePlacesModel;

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
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);


private slots:
    void placesChanged();

private:
    virtual void timerEvent(QTimerEvent *event);
    virtual void onUpdateAllowedChanged();

    void reloadItems();

    AppItemList favoritesList;
    QBasicTimer m_timer; // we use timer to skip dataChanged events and don't reload items very often
    KFilePlacesModel *m_placesModel;
    bool m_isPlacesChanged;
};
