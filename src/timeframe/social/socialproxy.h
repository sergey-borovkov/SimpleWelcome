#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include "socialitem.h"
#include <QMetaType>


#include "pluginrequestreply.h"

class ISocialPlugin;
class ListModel;
class PluginModel;
class SocialItem;
class SocialDayModel;
class PluginRequestReply;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent = 0);
    ~SocialProxy();

    void setModel(SocialDayModel *model);
    ListModel *socialModel();

    PluginModel *pluginModel();

    Q_INVOKABLE int authorizedPluginCount() const;
    Q_INVOKABLE QString authorizedPluginName(int i) const;

public slots:
    void likeItem(const QString &id, const QString &pluginName);
    void dislikeItem(const QString &id, const QString &pluginName);
    void commentItem(const QString &message, const QString &parentId, const QString &pluginName);

    bool anyPluginsEnabled();
    void newItem(SocialItem *item);
    void newItems(QList<SocialItem *> items);
    void startSearch();

      // temporary solution to get plugin names in QML
    // this functionality should perphaps be made available
    // to  QML via models
private slots:
    void likeSuccess(PluginRequestReply *);
    void commentSuccess(PluginRequestReply *);

    /**
     * @brief Slot called on social network deauthorization
     */
    void deauthorized();

    /**
     * @brief Slot called on social network authorization
     */
    void authorized();

signals:
    void pluginAuthorized();
    void pluginDeauthorized();
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

    /**
     * @brief postComment
     * @param message HTML escaped message
     * @param parentId Id of parent post
     * @param pluginName
     * @return pointer to PluginRequestReply
     */
    PluginRequestReply *postComment(const QString &message, const QString &parentId, const QString &pluginName);

    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    SocialDayModel *m_socialModel;
    QSet<QString> m_idSet;
    QSet<QString> m_enabledPlugins;
    QString m_cachedComment;
};

#endif // SOCIALPROXY_H
