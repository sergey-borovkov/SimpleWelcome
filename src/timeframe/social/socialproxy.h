#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>

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

    /**
     * @brief Like object with given id.
     * @param id Id of object you want to like
     * @param pluginName Name of social plugin object originates from
     */
    Q_INVOKABLE PluginRequestReply *like(const QString &id, const QString &pluginName);

    /**
     * @brief postComment
     * @param message HTML escaped message
     * @param parentId Id of parent post
     * @param pluginName
     * @return pointer to PluginRequestReply
     */
    Q_INVOKABLE PluginRequestReply *postComment(const QString &message, const QString &parentId, const QString &pluginName);

public slots:
    void authorized();
    void deauthorized();
    bool anyPluginsEnabled();
    void newItem(SocialItem *item);
    void newItems(QList<SocialItem *> items);
    void startSearch();

      // temporary solution to get plugin names in QML
    // this functionality should perphaps be made available
    // to  QML via models
    int authorizedPluginCount() const;
    QString authorizedPluginName(int i) const;

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

    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    SocialDayModel *m_socialModel;
    QSet<QString> m_idSet;
    QSet<QString> m_enabledPlugins;
};

#endif // SOCIALPROXY_H
