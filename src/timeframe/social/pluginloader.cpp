#include "pluginloader.h"
#include "socialplugin.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>

QList<ISocialModule*> PluginLoader::loadPlugins()
{
    QDir modulesDir(QCoreApplication::applicationDirPath() + "/plugins");
    QList<ISocialModule*> plugins;

    foreach (QString fileName, modulesDir.entryList(QDir::AllEntries))
    {
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *libObject = loader.instance();
        if (ISocialModule *lib = qobject_cast<ISocialModule*>(libObject))
            plugins << lib;
    }

    return plugins;
}
