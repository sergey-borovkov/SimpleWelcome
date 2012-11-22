#pragma once

#include <QtCore/QObject>

class DataSource : public QObject
{
    Q_OBJECT
public:
    DataSource(QObject *parent = 0);
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)
    Q_PROPERTY(bool isUpdateAllowed READ isUpdateAllowed WRITE setUpdateAllowed NOTIFY updateAllowedChanged)

    QString getQmlGroupName() {return qmlGroupName;}
    void setQmlGroupName(QString inQmlGroupName) {qmlGroupName = inQmlGroupName;}

    void setUpdateAllowed(bool allow);
    bool isUpdateAllowed() const { return m_isUpdateAllowed; }

signals:
    void resetContent();
    void updateItemData(int id, QString field, QString data);
    void runDesktopFile(QString desktopFile);
    void updateAllowedChanged();

protected:
    QString qmlGroupName;

private:
    virtual void onUpdateAllowedChanged() {}

    bool m_isUpdateAllowed; // to disallow updates if window is shown
};
