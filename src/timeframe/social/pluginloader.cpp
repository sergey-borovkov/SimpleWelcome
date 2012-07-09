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
    QDir modulesDir("/usr/share/timeframe");
    QList<ISocialModule*> plugins;
    qDebug() << "Loading plugins";
    foreach (QString fileName, modulesDir.entryList(QStringList() << "*.so", QDir::Files))
    {
        QPluginLoader loader("/usr/share/timeframe/" + fileName);
        QObject *libObject = loader.instance();
        if (ISocialModule *lib = qobject_cast<ISocialModule*>(libObject))
            plugins << lib;
    qDebug() << loader.errorString();
    }

    return plugins;
}
