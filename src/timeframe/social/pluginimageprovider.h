#ifndef PLUGINIMAGEPROVIDER_H
#define PLUGINIMAGEPROVIDER_H

#include "socialplugin.h"

#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtCore/QDebug>

class PluginImageProvider : public QDeclarativeImageProvider
{
public:
    PluginImageProvider(const QList<ISocialModule *> &plugins)
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
          m_plugins(plugins)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        foreach(ISocialModule *plugin, m_plugins)
        {
            if(plugin->name() == id)
                return plugin->icon();
        }

        return QPixmap();
    }
private:
    const QList<ISocialModule *> m_plugins;
};

#endif // PLUGINIMAGEPROVIDER_H
