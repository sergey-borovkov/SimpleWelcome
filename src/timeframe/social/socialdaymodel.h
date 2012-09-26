#ifndef SOCIALDAYMODEL_H
#define SOCIALDAYMODEL_H

#include <QtCore/QDate>
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>

class SocialDayItem;
class SocialItem;
class CommentItem;

class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel(QObject * parent = 0);

public slots:
    void setFilter(const QString &filter);
    int getIndexByDate(int year, int month,  bool direction);
    QObject* itemsModel(QDate date) const;
    QDate getDateOfIndex(int listIndex);

};

class SocialDayModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);

    void appendRows(const QList<SocialDayItem*> &items);
    void insertRow(int row, SocialDayItem* item);

public slots:
    void newSocialItems(QList<SocialItem*> list);
    QObject* itemsModel(QDate date) const;
    void likeItem(QString eventId);
    void addCommentToItem(CommentItem *, QString);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &eventId);
    void addComments(QString id, QList<CommentItem*> list);
    void removeItems(const QString &type);

private slots:
    void handleItemChange();

private:
    SocialDayItem *findItemByDate(const QDate &date) const;

    QHash<QString, QDate> m_idHash;
    QSet<QString> m_idSet;
};

#endif // SOCIALDAYMODEL_H
