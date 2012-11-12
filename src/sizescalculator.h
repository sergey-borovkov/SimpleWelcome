#pragma once

#include <QObject>

class QmlApplicationViewer;

class SizesCalculator : public QObject
{
    Q_OBJECT

    // Constants
    Q_PROPERTY(int textToGridSpacing READ textToGridSpacing CONSTANT)
    Q_PROPERTY(int groupTextHeight READ groupTextHeight CONSTANT)
    Q_PROPERTY(int gridWithGroupsSpacing READ gridWithGroupsSpacing CONSTANT)
    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT)

    // Calculated
    Q_PROPERTY(int cellWidth READ cellWidth NOTIFY iconSizeChanged) int cellWidth() const;
    Q_PROPERTY(int cellHeight READ cellHeight NOTIFY iconSizeChanged) int cellHeight() const;
    Q_PROPERTY(int iconTextSize READ iconTextSize NOTIFY iconSizeChanged) int iconTextSize() const;
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconSizeChanged) int iconSize() const;

public:
    SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer);

    int textToGridSpacing() const
    {
        return 16;
    }

    int groupTextHeight() const
    {
        return 24;
    }

    int gridWithGroupsSpacing() const
    {
        return 32;
    }

    int gridColumns() const
    {
        return 7;
    }

signals:
    void iconSizeChanged();

private:
    QmlApplicationViewer *viewer;

    int availableHeight() const;
};
