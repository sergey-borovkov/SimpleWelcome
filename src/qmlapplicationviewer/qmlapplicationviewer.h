#pragma once

#include <QtDeclarative/QDeclarativeView>
#include <QtCore/QVariantList>

class QResizeEvent;
class SizesCalculator;

class QmlApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

    Q_PROPERTY(QRect availableGeometry READ availableGeometry NOTIFY availableGeometryChanged)
    Q_PROPERTY(bool visible READ isVisible NOTIFY visibleChanged)

public:
    explicit QmlApplicationViewer(QWidget *parent = 0);

    /// @return available space in screen (relative to screen)
    QRect availableGeometry() const { return m_availGeometry; }

    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void moveEvent(QMoveEvent *);

    Q_INVOKABLE void activateDragAndDrop(QString url, QString image_path, int image_size);

    Q_INVOKABLE void saveStacks(QVariantList setting);
    Q_INVOKABLE void saveIconPositions(QVariantMap setting);
    Q_INVOKABLE QVariantMap loadStacks() const;
    Q_INVOKABLE QVariantMap loadIconPositions() const;

    Q_INVOKABLE void currentTabChanged(int newCurrentIndex);

    void setSizesCalculator(SizesCalculator *consts);

signals:
    void availableGeometryChanged();
    void windowHidden();
    void windowShown();
    void iconSizeChanged();
    void visibleChanged();

public slots:
    void updateWorkArea(int screen);
    void onScreenSizeChanged(int screen);
    void focusChanged(QWidget *, QWidget *now);


private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

    int currentTabIndex;
    int m_screen;
    QRect m_availGeometry;
    SizesCalculator *constants;
};
