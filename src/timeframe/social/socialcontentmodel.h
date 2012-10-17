#ifndef SOCIALCONTENTMODEL_H
#define SOCIALCONTENTMODEL_H

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtGui//QSortFilterProxyModel>

#include <listmodel.h>

class SocialContentItem;
class SocialItem;
class CommentItem;
class QDate;

class SocialContentModel : public ListModel
{
    Q_OBJECT
public:

    explicit SocialContentModel(QHash<int, QByteArray> roles, QObject *parent = 0);

    void appendRows(const QList<SocialContentItem*> &items);
    void insertRow(int row, SocialContentItem* item);

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
    SocialContentItem *findItemByDate(const QDate &date) const;

    QHash<QString, QDate> m_idHash;
    QSet<QString> m_idSet;
};

class SocialContentFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialContentFilterModel(QObject * parent = 0);

    Q_INVOKABLE QObject* itemsModel(QDate date) const;
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
    Q_INVOKABLE int getIndexByDate(int year, int month,  bool direction) const;

public slots:
    void setFilter(const QString &filter);
};

#endif // SOCIALCONTENTMODEL_H
