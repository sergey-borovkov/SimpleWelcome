#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include "pluginrequestreply.h"
#include "socialitem.h"

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSet>


class ISocialPlugin;
class ListModel;
class PluginModel;
class SocialItem;
class CommentItem;
class SocialContentModel;
class PluginRequestReply;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent = 0);
    ~SocialProxy();

    void setSocialModel(SocialContentModel *model);
    ListModel *socialModel();

    PluginModel *pluginModel();

    /**
     * @brief Like item with given id
     * @param id of item to like
     * @param plugin name
     */
    Q_INVOKABLE void likeItem(const QString &id, const QString &pluginName);

    /**
     * @brief Unlike already liked item
     * @param id of item to unlike
     * @param plugin name
     */
    Q_INVOKABLE void unlikeItem(const QString &id, const QString &pluginName);

    /**
     * @brief logout from social network and remove it's data from models
     * @param pluginName
     */
    Q_INVOKABLE void logout(const QString &pluginName);

    /**
     * @brief login in social network
     * @param pluginName
     */
    Q_INVOKABLE void login(const QString &pluginName);

    /**
     * @brief Comment item
     * @param message
     * @param post id
     * @param plugin name
     */
    Q_INVOKABLE void commentItem(const QString &message, const QString &parentId, const QString &pluginName);

    Q_INVOKABLE void getUserPicture(const QString &id, const QString &parentId, const QString &pluginName);

    Q_INVOKABLE int authorizedPluginCount() const;
    Q_INVOKABLE QString authorizedPluginName(int i) const;
    Q_INVOKABLE QString authorizedLocalizedPluginName(int i) const;
    Q_INVOKABLE bool anyPluginsEnabled();

    Q_INVOKABLE QString selfId(const QString &pluginName);
    Q_INVOKABLE QString selfName(const QString &pluginName);
    Q_INVOKABLE QString selfPictureUrl(const QString &pluginName);

    void getSelfUserPicture(const QString &pluginName);

    /**
     * @brief Request all comments of item
     * @param id of item
     * @param plugin name
     */
    Q_INVOKABLE PluginRequestReply *getAllComments(const QString &id, const QString &pluginName);

    Q_INVOKABLE PluginRequestReply *getAllLikes(const QString &id, const QString &pluginName);

public slots:
    void newItem(SocialItem *item);
    void newItems(QList<SocialItem *> items);

    void onSelfId(QString);
    void onSelfName(QString);
    void gotUserPictureUrl(QString, QString);
    void onSelfLiked(QString);
    void newComments(QString postId, QList<CommentItem *> items);
    void startSearch();

private slots:
    void likeSuccess(PluginRequestReply *);
    void commentSuccess(PluginRequestReply *);
    void getPictureSuccess(PluginRequestReply *);
    void getSelfPictureSuccess(PluginRequestReply*);

    /**
     * @brief Slot called on social network deauthorization
     */
    void deauthorized();

    /**
     * @brief Slot called on social network authorization
     */
    void authorized();

    void searchComplete();

signals:
    void pluginAuthorized();
    void pluginDeauthorized();
    void searchFinished();
    void searchStarted();

    /**
     * @brief This signal is for interacting with timescale model. Perphaps later it should
     *        be replace by a more proper solution
     * @param type
     */
    void removeType(QString type);
    void newMonth(int, int, QString);

private:

    /**
     * @brief Returns plugin with associated name. It's assumed that plugin calee looks for
     *        exists.
     *
     * @param pluginName
     * @return pointer to found plugin
     */
    ISocialPlugin *pluginFromName(const QString &pluginName);

    /**
     * @brief Like object with given id.
     * @param id Id of object you want to like
     * @param pluginName Name of social plugin object originates from
     */
    PluginRequestReply *like(const QString &id, const QString &pluginName);


    PluginRequestReply *dislike(const QString &id, const QString &pluginName);

    PluginRequestReply *getComments(const QString &id, const QString &pluginName);

    /**
     * @brief postComment
     * @param message HTML escaped message
     * @param parentId Id of parent post
     * @param pluginName
     * @return pointer to PluginRequestReply
     */
    PluginRequestReply *postComment(const QString &message, const QString &parentId, const QString &pluginName);

    PluginRequestReply *userPicture(const QString &id, const QString &parentId, const QString &pluginName);

    PluginRequestReply *selfPicture(const QString &pluginName);

    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    SocialContentModel *m_socialModel;
    QSet<QString> m_enabledPlugins;

    QString m_cachedComment;

    int m_searchInProgressCount;
};

#endif // SOCIALPROXY_H
