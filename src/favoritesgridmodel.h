#pragma once

#include <QAbstractListModel>

class AppItem;

class FavoritesGridModel : public QAbstractListModel
{
  Q_OBJECT

public:
    enum
    {
        CaptionRole = Qt::UserRole + 1,
        ImagePathRole
    };

    explicit FavoritesGridModel(QObject* parent = 0);
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

public slots:
    void itemClicked(int newIndex);

private:
    QList<AppItem> GetList(QString currentGroup) const;
    QString currentGroup;
};
