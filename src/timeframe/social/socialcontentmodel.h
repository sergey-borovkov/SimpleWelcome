#ifndef SOCIALCONTENTMODEL_H
#define SOCIALCONTENTMODEL_H

#include <QtCore/QHash>
#include <QtCore/QSet>
#include <QtCore/QDate>
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

    Q_INVOKABLE QObject* itemsModel(QDate date) const;
    void resetModel();

public slots:
    void addComments(QString id, QList<CommentItem*> list);
    void setSelfLiked(QString id);
    void addCommentToItem(CommentItem *, QString);
    void likeItem(QString eventId);
    void newSocialItems(QList<SocialItem*> list);
    void removeItems(const QString &type);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &eventId);
    void updateUserName(const QString &userId, const QString &userName, const QString &eventId);
    void updateAudioUrl(const QString &audioId, const QString &audioOwnerId, const QString &audioUrl, const QString &eventId);
    void updateVideoUrl(const QString &videoId, const QString &videoOwnerId, const QString &videoUrl, const QString &videoImage, const QString &eventId);

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
    Q_INVOKABLE void setFilter(const QString &filter);
    Q_INVOKABLE void resetModel();
    Q_INVOKABLE QObject* itemsModel(QDate date) const;
    Q_INVOKABLE QDate getDateOfIndex(int listIndex);
    Q_INVOKABLE int getIndexByDate(int year, int month,  bool direction) const;
    Q_INVOKABLE int getIndexByDate(QDate date);
    Q_INVOKABLE int count();

};

#endif // SOCIALCONTENTMODEL_H
