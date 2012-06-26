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
        QPluginLoader loader("lib/" + fileName);
        QObject *libObject = loader.instance();
        if (ISocialModule *lib = qobject_cast<ISocialModule*>(libObject))
        {
            plugins << lib;
            bool b = connect(lib, SIGNAL(authorizationStatusChanged(int)), SLOT(authorizationChanged(int)));
            qDebug() << b << "connect" ;
            QWidget *wind = lib->authenticationWidget();
            if(wind)
            {
                connect(wind, SIGNAL(destroyed()), SLOT(authorizationChanged()));
                wind->show();
            }
        }
    }
}

void PluginLoader::authorizationChanged(int status)
{
    qDebug() << "WE ARE FUCKING HERE";
    if(status == ISocialModule::Success)
        plugins[0]->requestManager()->queryWall(QDate(), QDate());
}

void PluginLoader::authorizationChanged()
{
    qDebug() << "WE ARE FUCKING HERE";
    plugins[0]->requestManager()->queryWall(QDate(), QDate());
}
