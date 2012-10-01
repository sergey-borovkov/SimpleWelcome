#ifndef PLUGINIMAGEPROVIDER_H
#define PLUGINIMAGEPROVIDER_H

#include "socialplugin.h"

#include <QtDeclarative/QDeclarativeImageProvider>

class PluginImageProvider : public QDeclarativeImageProvider
{
public:
    PluginImageProvider(const QList<ISocialPlugin *> &plugins)
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
          m_plugins(plugins) {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
        QString s = id;
        bool isSmall = false;
        if (s.right(6) == "/small") {
            s.chop(6);
            isSmall = true;
        }
        foreach(ISocialPlugin * plugin, m_plugins) {
            if (plugin->name() == s) {
                QPixmap p;
                if (isSmall) {
                    p = plugin->smallIcon();
                } else {
                    p = plugin->icon();
                }
                if (requestedSize.isValid()) {
                    p = p.scaled(*size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    if (size)
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
