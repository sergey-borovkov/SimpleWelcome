#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>

class ISocialPlugin;
class ListModel;
class PluginModel;
class SocialItem;
class SocialDayModel;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent = 0);
    ~SocialProxy();
    void setModel( SocialDayModel *model );

    ListModel *socialModel();
    PluginModel *pluginModel();

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
    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    SocialDayModel *m_socialModel;
    QSet<QString> m_idSet;
    QSet<QString> m_enabledPlugins;
};

#endif // SOCIALPROXY_H
