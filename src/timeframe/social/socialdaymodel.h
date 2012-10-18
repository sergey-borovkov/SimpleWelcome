#ifndef SOCIALDAYMODEL_H
#define SOCIALDAYMODEL_H

#include <QtCore/QHash>
#include <QtCore/QDate>
#include <QtCore/QSet>
#include <QtGui/QSortFilterProxyModel>

#include <listmodel.h>

class SocialDayItem;
class SocialItem;
class CommentItem;
class QDate;

class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel(QObject * parent = 0);
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
    Q_INVOKABLE int getIndexByDate(int year, int month,  bool direction);
    Q_INVOKABLE int getIndexByDate(QDate date);
    Q_INVOKABLE void resetModel();
    Q_INVOKABLE int count();

public slots:
    void setFilter(const QString &filter);

    QObject* itemsModel(QDate date) const;


};

class SocialDayModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);

    void appendRows(const QList<SocialDayItem*> &items);
    void insertRow(int row, SocialDayItem* item);
    void resetModel();
    Q_INVOKABLE QObject* itemsModel(QDate date) const;

public slots:
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
