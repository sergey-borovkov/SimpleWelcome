#pragma once

#include <QtDeclarative/QDeclarativeView>
#include <QVariantList>

class QResizeEvent;

class QmlApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

public:
    explicit QmlApplicationViewer(QWidget *parent = 0);

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void moveEvent(QMoveEvent *);

    Q_INVOKABLE QRect getMargins();
    Q_INVOKABLE void activateDragAndDrop(QString url, QString image_path, int image_size);

    Q_INVOKABLE void saveStacks(QVariantList setting);
    Q_INVOKABLE void saveIconPositions(QVariantMap setting);
    Q_INVOKABLE QVariantMap loadStacks();
    Q_INVOKABLE QVariantMap loadIconPositions();

    Q_INVOKABLE void currentTabChanged(int newCurrentIndex);

signals:
    void windowSizeChanged();
    void windowHidden();
    void windowShown();

public slots:
    void restore();
    void updateWorkArea();
    void focusChanged(QWidget *, QWidget *now);

private:
    int currentTabIndex;
};
