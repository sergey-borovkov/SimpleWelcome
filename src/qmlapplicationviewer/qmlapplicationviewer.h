#pragma once

#include <QtDeclarative/QDeclarativeView>
#include <QVariantList>

class QResizeEvent;

class QmlApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

    Q_PROPERTY(int updatableWidth READ updatableWidth NOTIFY windowSizeChanged)
    Q_PROPERTY(int updatableHeight READ updatableHeight NOTIFY windowSizeChanged)

public:
    explicit QmlApplicationViewer(QWidget *parent = 0);

    int updatableWidth() {
        return width();
    }
    int updatableHeight() {
        return height();
    }

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void moveEvent(QMoveEvent *event);

    Q_INVOKABLE QRect getMargins();
    Q_INVOKABLE void activateDragAndDrop(QString url, QString image_path, int image_size);
    Q_INVOKABLE void saveSetting(QString groupName, QVariantList setting);
    Q_INVOKABLE QVariantMap loadSetting(QString groupName);

signals:
    void windowSizeChanged();
    void windowHid();
    void windowShown();

public slots:
    void restore();
    void updateWorkArea();
};
