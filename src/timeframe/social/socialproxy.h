#ifndef SOCIALPROXY_H
#define SOCIALPROXY_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ISocialModule;
class SocialModel;

class SocialProxy : public QObject
{
    Q_OBJECT
public:
    explicit SocialProxy(QList<ISocialModule *> plugins, QObject *parent = 0);
    ~SocialProxy();
    void setModel(SocialModel *model);
    SocialModel *model() const;
signals:

public slots:
    void authenticated();
private:
    QList<ISocialModule *> m_plugins;
    SocialModel *m_model;
};

#endif // SOCIALPROXY_H
