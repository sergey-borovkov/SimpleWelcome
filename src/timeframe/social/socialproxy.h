#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ISocialModule;
class ListModel;
class SocialItem;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialModule *> plugins, QObject *parent = 0);
    ~SocialProxy();
    void setModel(ListModel *model);
    ListModel *socialModel();
    ListModel *pluginModel();

public slots:
    void authorized();
    void deauthorized();
    void newItem(SocialItem *item);
    void newItems(QList<SocialItem *> items);
private:
    QList<ISocialModule *> m_plugins;
    ListModel *m_pluginModel;
    ListModel *m_socialModel;
};

#endif // SOCIALPROXY_H
