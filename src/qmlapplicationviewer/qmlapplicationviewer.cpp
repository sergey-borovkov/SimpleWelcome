#include "qmlapplicationviewer.h"
#include <QResizeEvent>
#include <Plasma/WindowEffects>
#include <Plasma/Theme>
#include <KWindowSystem>
#include <QDebug>
#include <KConfigGroup>
#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <KWindowSystem>

QmlApplicationViewer::QmlApplicationViewer(QWidget *parent) :
    QDeclarativeView(parent), currentTabIndex(0)
{
    QDesktopWidget *desktop = QApplication::desktop();
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    connect(desktop, SIGNAL(workAreaResized(int)), SLOT(updateWorkArea(int)));
    connect(desktop, SIGNAL(resized(int)), SLOT(onScreenSizeChanged(int)));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setWindowFlags(Qt::FramelessWindowHint);

    setGeometry(desktop->screenGeometry(0));
    setFixedSize(size());

    // Window transparency
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background:transparent;");
}

void QmlApplicationViewer::resizeEvent(QResizeEvent *event)
{
    if (KWindowSystem::compositingActive()) { // && Plasma::Theme::defaultTheme()->windowTranslucencyEnabled())
        QRegion mask(QRect(QPoint(), size()));

        Plasma::WindowEffects::enableBlurBehind(winId(), true, mask);
        Plasma::WindowEffects::overrideShadow(winId(), true);
    }

    QDeclarativeView::resizeEvent(event);
    updateWorkArea(0);
}


void QmlApplicationViewer::updateWorkArea(int screen)
{
    if (screen == 0) {
        QRect screen_geom = QApplication::desktop()->screenGeometry(0);
        QRect avail_geom = QApplication::desktop()->availableGeometry(0);

        // compute rect relative to screen
        avail_geom.translate(-screen_geom.left(), -screen_geom.top());

        if (m_availGeometry != avail_geom) {
            m_availGeometry = avail_geom;
            emit availableGeometryChanged();
        }
    }
}

void QmlApplicationViewer::onScreenSizeChanged(int screen)
{
    if (screen == 0) {
        // resize main window to fill all screen
        setFixedSize(QApplication::desktop()->screenGeometry(0).size());
    }
}

void QmlApplicationViewer::focusChanged(QWidget *, QWidget *now)
{
    if (!now && currentTabIndex != 3) // When not on TimeFrame tab
        close();
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
    KWindowSystem::setState(winId(), NET::SkipTaskbar);
    show();
    activateWindow();
    //showFullScreen();
    //move(/*896*/0, 0);
}

void QmlApplicationViewer::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
    emit windowHidden();
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
    KConfigGroup configGroup(KGlobal::config(), "Stacks");
    configGroup.deleteGroup();

    foreach(QVariant variant, setting) {
        QVariantMap map;
        if (!strcmp(variant.typeName(), "QVariantMap")) {
            map = variant.value<QVariantMap>();

            QStringList stackList;
            QVariantList stack = map["stack"].toList();
            foreach(QVariant item, stack) {
                QVariantMap properties = item.toMap();
                stackList.append(properties["caption"].toString());
            }

            configGroup.writeEntry(map["caption"].toString(), stackList);
        }
    }

    configGroup.sync();
}

void QmlApplicationViewer::saveIconPositions(QVariantMap setting)
{
    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    configGroup.deleteGroup();

    QVariantMap::iterator it = setting.begin();
    for (; it != setting.end(); ++it) {
        configGroup.writeEntry(it.key(), it.value());
    }

    configGroup.sync();
}

QVariantMap QmlApplicationViewer::loadStacks() const
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


QVariantMap QmlApplicationViewer::loadIconPositions() const
{
    QVariantMap out;

    KConfigGroup configGroup(KGlobal::config(), "Apps positions");
    QMap<QString, QString> map = configGroup.entryMap();

    QMap<QString, QString>::iterator it = map.begin();
    for (; it != map.end(); ++it)
        out[it.key()] = it.value().toInt();

    return out;
}

void QmlApplicationViewer::currentTabChanged(int newCurrentIndex)
{
    currentTabIndex = newCurrentIndex;
}
