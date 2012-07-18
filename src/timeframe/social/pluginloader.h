#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QtCore/QList>

class ISocialPlugin;

class PluginLoader
{
public:
    QList<ISocialPlugin *> loadPlugins();
};

#endif // PLUGINLOADER_H
