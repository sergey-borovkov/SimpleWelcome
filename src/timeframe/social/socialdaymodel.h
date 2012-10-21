#ifndef SOCIALDAYMODEL_H
#define SOCIALDAYMODEL_H

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>
#include <QDate>

class SocialDayItem;
class SocialItem;
class CommentItem;
class QDate;

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
    QObject* itemsModel(QDate date) const;
    void addComments(QString id, QList<CommentItem*> list);
    void setSelfLiked(QString id);
    void addCommentToItem(CommentItem *, QString);
    void likeItem(QString eventId);
    void newSocialItems(QList<SocialItem*> list);
    void removeItems(const QString &type);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &eventId);
    void updateUserName(const QString &userId, const QString &userName, const QString &eventId);

private slots:
    void handleItemChange();

private:
    SocialDayItem *findItemByDate(const QDate &date) const;

    QHash<QString, QDate> m_idHash;
    QSet<QString> m_idSet;
};

#endif // SOCIALDAYMODEL_H
