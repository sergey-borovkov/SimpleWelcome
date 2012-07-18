#ifndef PLUGINIMAGEPROVIDER_H
#define PLUGINIMAGEPROVIDER_H

#include "socialplugin.h"

#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtCore/QDebug>

class PluginImageProvider : public QDeclarativeImageProvider
{
public:
    PluginImageProvider(const QList<ISocialPlugin *> &plugins)
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
          m_plugins(plugins)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        foreach(ISocialPlugin *plugin, m_plugins)
        {
            if(plugin->name() == id)
            {
                QPixmap p = plugin->icon();
                if(requestedSize.isValid())
                {
                    p = p.scaled(*size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    if(size)
                        *size = p.size();
                }
                return p;
            }
        }

        return QPixmap();
    }
private:
    const QList<ISocialPlugin *> m_plugins;
};

#endif // PLUGINIMAGEPROVIDER_H
