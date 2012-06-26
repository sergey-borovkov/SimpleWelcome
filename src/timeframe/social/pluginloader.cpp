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

void PluginLoader::loadPlugins()
{
    QDir modulesDir(QCoreApplication::applicationDirPath() + "/lib");

    foreach (QString fileName, modulesDir.entryList(QStringList() << "*.so", QDir::Files))
    {
        qDebug() << fileName;
        QPluginLoader loader("lib/" + fileName);
        QObject *libObject = loader.instance();
        qDebug() << loader.errorString();
        if (ISocialModule *lib = qobject_cast<ISocialModule*>(libObject))
        {
            QWidget *w = lib->authenticationWidget();
            if(w)
                w->show();
        }
    }
}
