#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMetaType>
#include <QtCore/QSet>
#include <QtCore/QDate>
#include <QtGui/QSortFilterProxyModel>

class Activity;

#pragma GCC diagnostic ignored "-Woverloaded-virtual"

/**
 * @brief The LocalDayModel class stores day's local contents
 *        and gives some helper members to be used from QML
 *        side
 */
class LocalDayModel : public QAbstractListModel
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
    explicit LocalDayModel(QDate date, QObject *parent = 0);
    ~LocalDayModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE QString url(int row) const;

    QDate date() const;
    void setDate(QDate date);

signals:
    void gotThumbnail(QString path);

public slots:
    void addActivityItem(Activity* item);

private:
    enum {
        ImageItemMax = 4,
        VideoItemMax = 2,
        DocumentItemMax = 1,
        AllItemMax = ImageItemMax + VideoItemMax + DocumentItemMax
    };

    int getImageInsertPosition(Activity *item, int from, int to);
    int getInsertPosition(Activity *item);
    void addActivityItemToCloud(Activity *item, int pos);
    void printFirstItems(int count);

private:
    QHash<int, QByteArray> m_hash;
    QList<Activity *> m_items;
    QSet<QString> m_urlSet;
    QHash<QString, QSize> m_imgDimensions;
    QDate m_date;

};

/**
 * @brief The LocalDayFilterModel class allows day's content
 *        kept in LocalDayModel to be filtered
 */
class LocalDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocalDayFilterModel(QObject * parent = 0);
    void setSourceModel(LocalDayModel * sourceModel);

    // for QML
    Q_INVOKABLE int count() const;
    Q_INVOKABLE QString url(int row) const;
    void previewReady(const QString &url);

signals:
    void gotThumbnail(QString path);
};


Q_DECLARE_METATYPE(LocalDayModel *)

#endif // ITEMMODEL_H
