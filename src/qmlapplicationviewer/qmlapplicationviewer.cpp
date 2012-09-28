// checksum 0x7895 version 0x4000b
/*
  This file was generated by the Qt Quick Application wizard of Qt Creator.
  QmlApplicationViewer is a convenience class containing mobile device specific
  code such as screen orientation handling. Also QML paths and debugging are
  handled here.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

#include "qmlapplicationviewer.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeImageProvider>

#include <QResizeEvent>

#if defined(QMLJSDEBUGGER)
#include <qt_private/qdeclarativedebughelper_p.h>
#endif

#if defined(QMLJSDEBUGGER) && !defined(NO_JSDEBUGGER)
#include <jsdebuggeragent.h>
#endif
#if defined(QMLJSDEBUGGER) && !defined(NO_QMLOBSERVER)
#include <qdeclarativeviewobserver.h>
#endif

#if defined(QMLJSDEBUGGER)

// Enable debugging before any QDeclarativeEngine is created
struct QmlJsDebuggingEnabler {
    QmlJsDebuggingEnabler() {
        QDeclarativeDebugHelper::enableDebugging();
    }
};

// Execute code in constructor before first QDeclarativeEngine is instantiated
static QmlJsDebuggingEnabler enableDebuggingHelper;

#endif // QMLJSDEBUGGER

class QmlApplicationViewerPrivate
{
    QString mainQmlFile;
    friend class QmlApplicationViewer;
    static QString adjustPath(const QString &path);
};

QString QmlApplicationViewerPrivate::adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if(!QDir::isAbsolutePath(path))
        return QCoreApplication::applicationDirPath()
               + QLatin1String("/../Resources/") + path;
#else
    const QString pathInInstallDir = QCoreApplication::applicationDirPath()
                                     + QLatin1String("/../") + path;
    if(pathInInstallDir.contains(QLatin1String("opt"))
            && pathInInstallDir.contains(QLatin1String("bin"))
            && QFileInfo(pathInInstallDir).exists()) {
        return pathInInstallDir;
    }
#endif
#endif
    return path;
}

QmlApplicationViewer::QmlApplicationViewer(QWidget *parent) :
    QDeclarativeView(parent),
    m_d(new QmlApplicationViewerPrivate)
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
#if defined(QMLJSDEBUGGER) && !defined(NO_JSDEBUGGER)
    new QmlJSDebugger::JSDebuggerAgent(engine());
#endif
#if defined(QMLJSDEBUGGER) && !defined(NO_QMLOBSERVER)
    new QmlJSDebugger::QDeclarativeViewObserver(this, this);
#endif
}

QmlApplicationViewer::~QmlApplicationViewer()
{
    delete m_d;
}

void QmlApplicationViewer::setMainQmlFile(const QString &file)
{
    m_d->mainQmlFile = QmlApplicationViewerPrivate::adjustPath(file);
    setSource(QUrl::fromLocalFile(m_d->mainQmlFile));
}

void QmlApplicationViewer::addImportPath(const QString &path)
{
    engine()->addImportPath(QmlApplicationViewerPrivate::adjustPath(path));
}

void QmlApplicationViewer::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if(orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if(v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch(orientation) {
#if QT_VERSION < 0x040702
        // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void QmlApplicationViewer::showExpanded()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

#include <Plasma/WindowEffects>
#include <Plasma/Theme>
#include <KWindowSystem>
#include <QDebug>

void QmlApplicationViewer::resizeEvent(QResizeEvent *event)
{
    emit windowSizeChanged(event->size().width(), event->size().height());

    if(KWindowSystem::compositingActive()) // && Plasma::Theme::defaultTheme()->windowTranslucencyEnabled())
    {
        QRegion mask(QRect(QPoint(), size()));

        Plasma::WindowEffects::enableBlurBehind(winId(), true, mask);
        Plasma::WindowEffects::overrideShadow(winId(), true);
    }

    QDeclarativeView::resizeEvent(event);
}

void QmlApplicationViewer::closeEvent(QCloseEvent *event)
{
    if (1)
    {
        event->ignore();
        hide();
        emit windowHid();
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

    if (eng ) {
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

#include <KConfigGroup>

void QmlApplicationViewer::saveSetting(QString groupName, QVariantList setting)
{
    qDebug() << "WE ARE HERE WE ARE SAVING" << groupName;

    KConfigGroup configGroup(KGlobal::config(), "Stacks"); // FIXME: not to depend on locale
    configGroup.deleteGroup();

    foreach (QVariant variant, setting)
    {
        QVariantMap map;
        if (!strcmp(variant.typeName(), "QVariantMap"))
        {
            qDebug() << "REALLY QVariantMap";
            map = variant.value<QVariantMap>();
            //qDebug() << map["caption"].toString();
            //qDebug() << map["imagePath"].toString();
            //qDebug() << map["stack"];


            QStringList stackList;
            QVariantList stack = map["stack"].toList();
            foreach (QVariant item, stack)
            {
                QVariantMap properties = item.toMap();
                stackList.append(properties["caption"].toString());
            }

            configGroup.writeEntry(map["caption"].toString(), stackList);
            //qDebug() << "SIZE" << map["stack"].toList().size();
        }
    }

    configGroup.sync();
}
