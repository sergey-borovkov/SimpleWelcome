#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QtCore/QList>
#include <QtCore/QString>

class ISocialPlugin;

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
