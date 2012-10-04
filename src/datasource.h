#pragma once

#include <QObject>

class DataSource : public QObject
{
public:
    DataSource(QObject *parent = 0)
        : QObject(parent) {}
    //Q_INVOKABLE virtual int getItemCount(QString group = "") = 0;
public slots:
    //virtual void itemClicked(int newIndex) = 0;
    virtual void getContent() = 0;

signals:
    //void newItemData(QString iconPath, QString name, int id) const;
    void resetContent();
    void updateItemData(int id, QString field, QString data);
    void runDesktopFile(QString desktopFile);
};
