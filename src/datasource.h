#pragma once

#include <QObject>

class DataSource : public QObject
{
public:
    DataSource(QObject *parent = 0)
        : QObject(parent) {}
    //Q_INVOKABLE virtual int getItemCount(QString group = "") = 0;
    Q_PROPERTY(QString qmlGroupName READ getQmlGroupName WRITE setQmlGroupName)

    QString getQmlGroupName() {return qmlGroupName;}
    void setQmlGroupName(QString inQmlGroupName) {qmlGroupName = inQmlGroupName;}

public slots:
    //virtual void itemClicked(int newIndex) = 0;
    virtual void getContent() = 0;

signals:
    //void newItemData(QVariantMap itemData) const;
    void resetContent();
    void updateItemData(int id, QString field, QString data);
    void runDesktopFile(QString desktopFile);

protected:
    QString qmlGroupName;
};
