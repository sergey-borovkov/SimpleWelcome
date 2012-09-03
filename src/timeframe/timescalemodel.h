#ifndef TIMESCALEMODEL_H
#define TIMESCALEMODEL_H

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QHash>
#include <QSet>
#include <QPair>


class ActivityProxy;

/*
class ActivityList;
class ActivitySet;
*/

class TimeScaleFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TimeScaleFilterModel(QObject * parent = 0);
protected:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const;
public slots:
    void setFilter(const QString &filter);
    int getYear(int ind);
    int getMonth(int ind);
    int count();
};

class TimeScaleItem : public QObject
{
    Q_OBJECT
public:
    enum Roles {
        YearRole = Qt::UserRole + 1,
        MonthRole,
        TypesRole
    };
public:
    TimeScaleItem(QObject *parent = 0): QObject(parent) {}
    explicit TimeScaleItem(int year, int month, QString type, QObject* parent = 0);
    int year() const;
    int month() const;
    QString types() const;
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    void addType(QString type);
signals:
    void dataChanged();
private:
    int m_year;
    int m_month;
    QString m_type;
};

class TimeScaleModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TimeScaleModel(TimeScaleItem* prototype, QObject* parent = 0);
    ~TimeScaleModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void appendRow(TimeScaleItem* item);
    void appendRows(const QList<TimeScaleItem*> &items);
    void insertRow(int row, TimeScaleItem* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    TimeScaleItem* takeRow(int row);
    TimeScaleItem* find(const int year, const int month);
    QModelIndex indexFromItem(const TimeScaleItem* item) const;
    void clear();

public slots:
    void newItem(int year, int month, QString type);

private slots:
    void handleItemChange();

private:
    QSet <QPair <int, int> > m_dates;
    TimeScaleItem* m_prototype;
    QList<TimeScaleItem*> m_list;

};

#endif // ACTIVITYMODEL_H
