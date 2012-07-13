#ifndef PLUGINIMAGEPROVIDER_H
#define PLUGINIMAGEPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

class PluginImageProvider : public QDeclarativeImageProvider
{
    PluginImageProvider()
        : QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
    {
    }

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
    {
        int width = 100;
        int height = 50;

        if (size)
            *size = QSize(width, height);
        QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                       requestedSize.height() > 0 ? requestedSize.height() : height);
        pixmap.fill(QColor(id).rgba());

        return pixmap;
    }
};

#endif // PLUGINIMAGEPROVIDER_H
