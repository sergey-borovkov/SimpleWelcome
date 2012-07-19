#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ISocialPlugin;
class ListModel;
class PluginModel;
class SocialItem;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialPlugin *> plugins, QObject *parent = 0);
    ~SocialProxy();
    void setModel(ListModel *model);

    ListModel *socialModel();
    PluginModel *pluginModel();

public slots:
    void authorized();
    void deauthorized();
    bool anyPluginsEnabled();
    void newItem(SocialItem *item);
    void newItems(QList<SocialItem *> items);
    void startSearch();

private:
    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    ListModel *m_socialModel;
    bool m_anyEnabled; // true if any plugins enabled and authorized
};

#endif // SOCIALPROXY_H
