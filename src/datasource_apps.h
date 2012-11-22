#pragma once

#include "appitem.h"
#include "datasource.h"

class DataSource_RecentApps;

class DataSource_Apps : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

public:
    explicit DataSource_Apps(QObject *parent = 0, DataSource_RecentApps *recentApps = 0);
    Q_INVOKABLE virtual int getItemCount();
    Q_INVOKABLE virtual QVariantMap getContent(int index);

    Q_INVOKABLE QString itemUrlDnd(int id);

    void setUpdateAllowed(bool allow);

signals:
    void newItemData(QVariantMap itemData, QString group);
    void resetContent();
    void runDesktopFile(QString desktopFile);

public slots:
    virtual void itemClicked(int newIndex);

private slots:
    void ksycocaChanged(const QStringList &changes);

private:
    void updateItems(bool isResetContent = true);
    void saveData();

    AppItemList appsList;
    QString currentGroup;
    QString prevCurrentGroup;

    DataSource_RecentApps *recentApps;
    bool m_isDbChanged;  // indicates that DB of application is changed
    bool m_isUpdateAllowed; // allows updates of application (initially: to disallow update while window is shown)
};
