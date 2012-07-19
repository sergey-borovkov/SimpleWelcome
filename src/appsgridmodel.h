#pragma once

#include <QAbstractListModel>
#include "appitem.h"

class AppsGridModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum
    {
        CaptionRole = Qt::UserRole + 1,
        ImagePathRole
    };

    explicit AppsGridModel(QObject* parent = 0);
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

    QList<AppItem> GetList(QString currentGroup) const;

public slots:
    void itemClicked(int newIndex);
    void updateContent();

private:
    QString currentGroup;

signals:
    void contentChanged();
    void newItemData(QString iconPath, QString name) const;
    void dataClear();
};
