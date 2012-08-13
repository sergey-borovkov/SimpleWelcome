#include "pluginloader.h"
#include "socialplugin.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

QList<ISocialPlugin*> PluginLoader::loadPlugins(QString dir_path)
{
    QDir modulesDir(dir_path);
    QList<ISocialPlugin*> plugins;

    foreach (QString fileName, modulesDir.entryList(QDir::AllEntries))
    {
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *libObject = loader.instance();
        if (ISocialPlugin *lib = qobject_cast<ISocialPlugin*>(libObject))
            plugins << lib;
    }

    return plugins;
}
