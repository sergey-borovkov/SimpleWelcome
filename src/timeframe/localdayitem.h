#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QObject>
#include <QMetaType>
#include <QStringList>
#include <QDate>
#include <QSortFilterProxyModel>

class ItemModel;
//class QSortFilterProxyModel;
class Activity;
//class QStringList;


class TimeFrameFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeFrameFilterModel(QObject * parent = 0);
    virtual void setSourceModel ( ItemModel * sourceModel );

signals:
    void gotThumbnail();

public slots:
    QString url(int row);

};


class LocalDayItem : public QObject
{
    Q_OBJECT
public:
    explicit LocalDayItem(const QDate &date, QObject *parent = 0);
    ~LocalDayItem();

    QDate getDate();
    int   getCount();
    //ItemModel * model();
    TimeFrameFilterModel * model();
    QString types() const;
    void thumbnailReady(QString);
    void setActivityFilter(const QRegExp&);

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
