#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QtCore/QList>

class ISocialModule;

class PluginLoader
{
public:
    PluginLoader();
    QList<ISocialModule*> loadPlugins();
};

#endif // PLUGINLOADER_H