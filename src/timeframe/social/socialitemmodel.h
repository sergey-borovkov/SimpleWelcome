#ifndef SOCIALITEMMODEL_H
#define SOCIALITEMMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>

#include <listmodel.h>

class SocialItem;
class CommentItem;

class SocialItemModel : public ListModel
{
    Q_OBJECT

public:
    explicit SocialItemModel(QHash<int, QByteArray> roles, QObject *parent = 0);
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

Q_DECLARE_METATYPE(SocialItemModel *)

#endif // SOCIALITEMMODEL_H
