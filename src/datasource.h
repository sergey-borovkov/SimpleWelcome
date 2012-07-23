#pragma once

#include <QObject>

class DataSource : public QObject
{
public:
    DataSource(QObject *parent = 0)
        : QObject(parent) {}

public slots:
    virtual void itemClicked(int newIndex) = 0;
    virtual void updateContent() = 0;

signals:
    void newItemData(QString iconPath, QString name, int id) const;
    void dataClear();
};
