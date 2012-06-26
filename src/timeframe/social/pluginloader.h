#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QtCore/QObject>
#include <QtCore/QList>

class ISocialModule;

class PluginLoader : public QObject
{
    Q_OBJECT
public:
    PluginLoader();
    void loadPlugins();

public slots:
    void authorizationChanged(int status);
    void authorizationChanged();
private:
    QList<ISocialModule*> plugins;
};

#endif // PLUGINLOADER_H
