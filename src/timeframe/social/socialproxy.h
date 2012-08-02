#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSet>

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

    // temporary to get plugin names in QML
    // this functionality should perphaps be made available
    // to  QML via models
    int count() const;
    QString name(int i);

signals:
    void pluginAuthorized();
    void newMonth(int, int, QString);

private:
    QList<ISocialPlugin *> m_plugins;
    PluginModel *m_pluginModel;
    SocialDayModel *m_socialModel;
    QSet<QString> m_idSet;

    bool m_anyEnabled; // true if any plugins enabled and authorized
};

#endif // SOCIALPROXY_H
