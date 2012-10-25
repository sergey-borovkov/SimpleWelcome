#include "qmlapplicationviewer.h"
#include <QResizeEvent>
#include <Plasma/WindowEffects>
#include <Plasma/Theme>
#include <KWindowSystem>
#include <QDebug>
#include <KConfigGroup>
#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtDeclarative/QDeclarativeEngine>
#include <QDesktopWidget>
#include <QApplication>
#include <KWindowSystem>

QmlApplicationViewer::QmlApplicationViewer(QWidget *parent) :
    QDeclarativeView(parent)
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), SLOT(updateWorkArea()));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

void QmlApplicationViewer::resizeEvent(QResizeEvent *event)
{
//    if (KWindowSystem::compositingActive()) { // && Plasma::Theme::defaultTheme()->windowTranslucencyEnabled())
//        QRegion mask(QRect(QPoint(), size()));

//        Plasma::WindowEffects::enableBlurBehind(winId(), true, mask);
//        Plasma::WindowEffects::overrideShadow(winId(), true);
//    }

    QDeclarativeView::resizeEvent(event);
    updateWorkArea();
}


void QmlApplicationViewer::updateWorkArea()
{
    qDebug() << "Work area update";

    emit windowSizeChanged();
}

QRect QmlApplicationViewer::getMargins()
{
    QRect result(QApplication::desktop()->availableGeometry().left(), QApplication::desktop()->availableGeometry().top(),
                 QApplication::desktop()->width() - QApplication::desktop()->availableGeometry().right(),
                 QApplication::desktop()->height() - QApplication::desktop()->availableGeometry().bottom());
    return result;
}

void QmlApplicationViewer::moveEvent(QMoveEvent *)
{
    //qDebug() << "-- Move to " << event->pos().x() << ":" << event->pos().y();
    move(0, 0);
}

void QmlApplicationViewer::restore()
{
    emit windowShown();

    //setGeometry(896, 0, 1600, 900);//1280, 1024); // 1000); //
    KWindowSystem::setState( winId(), NET::SkipTaskbar );
    show();
    activateWindow();
    //showFullScreen();
    //move(/*896*/0, 0);
}

void QmlApplicationViewer::closeEvent(QCloseEvent *event)
{
    if (1) {
        event->ignore();
        hide();
        emit windowHidden();
    }
}

void QmlApplicationViewer::activateDragAndDrop(QString url, QString image_path, int image_size)
{
    QDeclarativeEngine *eng = engine();

    QMimeData *mime = new QMimeData;
    QList<QUrl> urls;
    urls.append(QUrl(url));
    mime->setUrls(urls);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);

    if (eng) {
        image_path.remove(QString::fromLatin1("image://"));
        // grep name of image provider
        QString provider_name = image_path.section(QChar::fromAscii('/'), 0, 0);
        QString image_id = image_path.section(QChar::fromAscii('/'), 1);
        QDeclarativeImageProvider *provider = eng->imageProvider(provider_name);
        if (provider && provider->imageType() == QDeclarativeImageProvider::Pixmap) {
            QSize size;
            QPixmap pixmap = provider->requestPixmap(image_id, &size, QSize(image_size, image_size));
            drag->setPixmap(pixmap);
        }
    }

    drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, Qt::CopyAction);
}

void QmlApplicationViewer::saveStacks(QVariantList setting)
{
    //qDebug() << "WE ARE HERE WE ARE SAVING stacking";

    KConfigGroup configGroup(KGlobal::config(), "Stacks");
    configGroup.deleteGroup();

    foreach(QVariant variant, setting) {
        QVariantMap map;
        if (!strcmp(variant.typeName(), "QVariantMap")) {
            //qDebug() << "REALLY QVariantMap";
            map = variant.value<QVariantMap>();
            //qDebug() << map["caption"].toString();
            //qDebug() << map["imagePath"].toString();
            //qDebug() << map["stack"];

            //qDebug() << "In" << map["caption"].toString();

            QStringList stackList;
            QVariantList stack = map["stack"].toList();
            foreach(QVariant item, stack) {
                QVariantMap properties = item.toMap();
                stackList.append(properties["caption"].toString());
                //qDebug() << " L" << properties["caption"].toString();
            }

            configGroup.writeEntry(map["caption"].toString(), stackList);
            //qDebug() << "SIZE" << map["stack"].toList().size();
        }
    }

    configGroup.sync();
}

void QmlApplicationViewer::saveIconPositions(QVariantMap setting)
{
    //qDebug() << "WE ARE HERE WE ARE SAVING" << setting;

    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    configGroup.deleteGroup();

    QVariantMap::iterator it = setting.begin();
    for (; it != setting.end(); ++it) {
        configGroup.writeEntry(it.key(), it.value());
    }

//    foreach(QVariant variant, setting) {
//        QVariantMap map;
//        if (!strcmp(variant.typeName(), "QVariantMap")) {
//            map = variant.value<QVariantMap>();
//            qDebug() << map;

//            //configGroup.writeEntry(map["caption"].toString(), stackList);
//            //qDebug() << "SIZE" << map["stack"].toList().size();
//        }
//    }

    configGroup.sync();
}

QVariantMap QmlApplicationViewer::loadStacks()
{
    QVariantMap list;

    KConfigGroup configGroup(KGlobal::config(), "Stacks");
    QMap<QString, QString > map = configGroup.entryMap();

    foreach(QString key, map.keys())
        list[key] = map[key];

    //qDebug() << "LOADED STACKS:";
    //qDebug() << list;
    return list;
}


QVariantMap QmlApplicationViewer::loadIconPositions()
{
    QVariantMap out;

    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    QMap<QString, QString> map = configGroup.entryMap();

    QMap<QString, QString>::iterator it = map.begin();
    for (; it != map.end(); ++it)
        out[it.key()] = it.value().toInt();

    return out;
}
