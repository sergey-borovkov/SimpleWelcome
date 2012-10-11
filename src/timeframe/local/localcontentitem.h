#ifndef GALLERYITEM_H
#define GALLERYITEM_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QStringList>
#include <QtCore/QDate>
#include <QtGui/QSortFilterProxyModel>

#include <listitem.h>

class LocalDayModel;
class LocalDayFilterModel;
class Activity;

/**
 * @brief The LocalContentItem class stores local content in one day
 */
class LocalContentItem : public QObject, public ListItem
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

    explicit LocalContentItem(const QDate &date, QObject *parent = 0);
    ~LocalContentItem();

    QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant & value);

    QDate getDate() const;
    int   getCount() const;

    LocalDayFilterModel * model();
    QString types() const;

    static QHash<int, QByteArray> roleNames();

signals:
    void dataChanged();

public slots:
    void setDate(const QDate&);
    void addActivity(Activity* item);

private:
    QString     m_types;
    int         m_count;
    LocalDayModel*  m_itemModel;
    LocalDayFilterModel*  m_model;
};

#endif // GALLERYITEM_H
