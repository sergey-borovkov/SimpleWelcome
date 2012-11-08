#pragma once

#include <QtDeclarative/QDeclarativeView>
#include <QVariantList>

class QResizeEvent;

class QmlApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

    Q_PROPERTY(QRect availableGeometry READ availableGeometry NOTIFY availableGeometryChanged)

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

signals:
    void availableGeometryChanged();
    void windowHidden();
    void windowShown();

public slots:
    void restore();
    void updateWorkArea(int screen);
    void onScreenSizeChanged(int screen);
    void focusChanged(QWidget *, QWidget *now);


private:
    int currentTabIndex;
    QRect m_availGeometry;
};
