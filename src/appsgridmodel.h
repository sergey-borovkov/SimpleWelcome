#pragma once

#include <QAbstractListModel>

class AppProvider;

class AppsGridModel : public QAbstractListModel
{
  Q_OBJECT

public:
    enum
    {
        CaptionRole = Qt::UserRole + 1,
        ImagePathRole
    };

    explicit AppsGridModel(AppProvider *appProvider, QObject* parent = 0);
    ~AppsGridModel();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

public slots:
    void changeFolder(int newIndex);

private:
    AppProvider *appProvider;
    QString currentGroup;
};
