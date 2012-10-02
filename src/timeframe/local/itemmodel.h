#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMetaType>
#include <QtCore/QSet>

class Activity;


class ItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum {
        CurrentDateRole = Qt::UserRole + 1,
        ActivitiesRole,
        CountRole,
        UrlRole,
        TypeRole
    };
    explicit ItemModel(QObject *parent = 0);
    ~ItemModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;

signals:
    void gotThumbnail();

public slots:
    void addActivityItem(Activity* item);
    void thumbnailReady(QString);

    QString url(int row);

private:
    QHash<int, QByteArray> m_hash;
    QList<Activity *> m_items;
    QSet <QString> m_urlSet;
};

Q_DECLARE_METATYPE(ItemModel *)

#endif // ITEMMODEL_H
