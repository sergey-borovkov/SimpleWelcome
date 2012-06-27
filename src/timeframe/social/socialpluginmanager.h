#ifndef SOCIALPLUGINMANAGER_H
#define SOCIALPLUGINMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ISocialModule;

class SocialPluginManager : public QObject
{
    Q_OBJECT
public:
    explicit SocialPluginManager(QList<ISocialModule *> plugins, QObject *parent = 0);

signals:

public slots:
    void authenticated();
private:
    QList<ISocialModule *> m_plugins;
};

#endif // SOCIALPLUGINMANAGER_H
