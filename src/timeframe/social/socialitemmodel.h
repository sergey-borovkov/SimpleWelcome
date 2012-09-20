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
    ~SocialItemModel();

signals:

public slots:
    void addSocialItem(SocialItem* item);
    void like(QString id);
    void addComment(CommentItem* item, QString id);

private:
    QSet<QString> m_idSet;

};

Q_DECLARE_METATYPE(SocialItemModel *)

#endif // SOCIALITEMMODEL_H
