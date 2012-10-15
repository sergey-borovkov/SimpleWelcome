#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QtCore/QList>

class ISocialPlugin;
class QString;

class PluginLoader
{
public:
    /**
     * Loads plugins from specified directory.
     * @param dir_path A path to directory containing plugins.
     */
    QList<ISocialPlugin *> loadPlugins(QString dir_path);
};

#endif // PLUGINLOADER_H
