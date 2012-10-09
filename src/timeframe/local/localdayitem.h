#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include <QtCore/QDate>
#include <QtGui/QSortFilterProxyModel>

#include <listitem.h>

class ItemModel;
class Activity;

#pragma GCC diagnostic ignored "-Woverloaded-virtual"

class TimeFrameFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeFrameFilterModel(QObject * parent = 0);
    void setSourceModel(ItemModel * sourceModel);
    Q_INVOKABLE QString url(int row) const;

signals:
    void gotThumbnail();


};


/**
 * @brief The LocalDayItem class stores local content in one day
 */
class LocalDayItem : public QObject, public ListItem
{
    Q_OBJECT
public:
    enum {
        CurrentDateRole = Qt::UserRole + 1,
        ItemsRole,
        CountRole,
        TypesRole,
        ItemsCountRole
    };

    explicit LocalDayItem(const QDate &date, QObject *parent = 0);
    ~LocalDayItem();

    QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant & value);

    QDate getDate() const;
    int   getCount() const;

    TimeFrameFilterModel * model();
    QString types() const;
    void thumbnailReady(QString);

    static QHash<int, QByteArray> roleNames();

signals:
    void dataChanged();

public slots:
    void setDate(const QDate&);
    void addActivity(Activity* item);

private:
    QString     m_types;
    QDate       m_date;
    int         m_count;
    ItemModel*  m_itemModel;
    TimeFrameFilterModel*  m_model;
};

#endif // GALLERYITEM_H
