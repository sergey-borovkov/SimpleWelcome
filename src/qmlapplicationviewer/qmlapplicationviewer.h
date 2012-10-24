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
    virtual void moveEvent(QMoveEvent *);

    Q_INVOKABLE QRect getMargins();
    Q_INVOKABLE void activateDragAndDrop(QString url, QString image_path, int image_size);

    Q_INVOKABLE void saveStacks(QVariantList setting);
    Q_INVOKABLE void saveIconPositions(QVariantMap setting);
    Q_INVOKABLE QVariantMap loadStacks();
    Q_INVOKABLE QVariantMap loadIconPositions();

signals:
    void windowSizeChanged();
    void windowHidden();
    void windowShown();

public slots:
    void restore();
    void updateWorkArea();
};
