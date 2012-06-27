#include "pluginloader.h"
#include "socialplugin.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QtDebug>
#include <QMessageBox>

PluginLoader::PluginLoader()
{
}

QList<ISocialModule*> PluginLoader::loadPlugins()
{
    QDir modulesDir(QCoreApplication::applicationDirPath() + "/lib");
    QList<ISocialModule*> plugins;

    foreach (QString fileName, modulesDir.entryList(QStringList() << "*.so", QDir::Files))
    {
        QPluginLoader loader("lib/" + fileName);
        QObject *libObject = loader.instance();
        if (ISocialModule *lib = qobject_cast<ISocialModule*>(libObject))
            plugins << lib;
    }

    return plugins;
}
