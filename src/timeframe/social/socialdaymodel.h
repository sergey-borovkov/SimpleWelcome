#ifndef SOCIALITEMMODEL_H
#define SOCIALITEMMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>

#include <listmodel.h>

class SocialItem;
class CommentItem;

class SocialDayModel : public ListModel
{
    Q_OBJECT

public:
    explicit SocialDayModel(QHash<int, QByteArray> roles, QObject *parent = 0);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

public slots:
    void addComment(CommentItem* item, QString id);
    void addComments(QString id, QList<CommentItem*> list);
    void setSelfLiked(QString id);
    void addSocialItem(SocialItem* item);
    void like(QString id);
    void updateUserImage(const QString &userId, const QString &userImageUrl, const QString &id);
    void updateUserName(const QString &userId, const QString &userName, const QString &id);

private:
    QSet<QString> m_idSet;
};

class SocialDayFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SocialDayFilterModel(QObject * parent = 0);

signals:
    /**
     * @brief updateData - signal is emitted for not-view-based delegates to update role values
     */
    void updateData();

public slots:
    QString id(int row) const;
    QString imageUrl(int row) const;
    QString text(int row) const;
    int likesCount(int row) const;
    int like(int row);
    int commentsCount(int row) const;
    QObject *comments(int row) const;
    QString pluginName(int row) const;
    void update();
};

Q_DECLARE_METATYPE(SocialDayModel *)

#endif // SOCIALITEMMODEL_H
