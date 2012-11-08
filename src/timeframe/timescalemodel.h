#ifndef TIMESCALEMODEL_H
#define TIMESCALEMODEL_H

#include <QtCore/QDate>
#include <QtGui/QSortFilterProxyModel>

#include <listmodel.h>
#include <listitem.h>

class ActivityProxy;


class TimeScaleFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeScaleFilterModel(QObject *parent = 0);
    Q_INVOKABLE void resetModel();
    Q_INVOKABLE int count();

protected:
//    bool lessThan(const QModelIndex & left, const QModelIndex & right) const;
public slots:
    void setFilter(const QString &filter);
    int getYear(int ind);
    int getMonth(int ind);
    int getDate(int ind);
};

class TimeScaleItem : public QObject, public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        YearRole = Qt::UserRole + 1,
        MonthRole,
        DateRole,
        TypesRole
    };
public:
    explicit TimeScaleItem(int year, int month, QString type, QObject *parent = 0);
    QString id() const;
    bool setData(int role, const QVariant &value) {
        Q_UNUSED(role)
        Q_UNUSED(value)
        return false;
    }
    int year() const;
    int month() const;
    QDate date() const;
    QString types() const;
    QVariant data(int role) const;
    static QHash<int, QByteArray> roleNames();

    void addType(QString type);
    void setType(QString types);
signals:
    void dataChanged();
private:
    int m_year;
    int m_month;
    QDate m_date;
    QString m_type;
};

class TimeScaleModel : public ListModel
{
    Q_OBJECT
public:
    explicit TimeScaleModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    TimeScaleItem *find(const int year, const int month);
    TimeScaleItem *find(const QDate &year);
public slots:
    void newItem(int year, int month, QString type);

    /**
     * @brief Update type fields when one of plugins logs out
     * @param type
     */
    void removeItems(const QString &type);

private slots:
    void handleItemChange();
};

#endif // ACTIVITYMODEL_H
