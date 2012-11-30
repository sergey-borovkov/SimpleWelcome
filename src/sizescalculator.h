#pragma once

#include <QObject>
#include <QSize>

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
    Q_PROPERTY(int cellWidth READ cellWidth NOTIFY iconSizeChanged)
    Q_PROPERTY(int cellHeight READ cellHeight NOTIFY iconSizeChanged)
    Q_PROPERTY(int iconTextSize READ iconTextSize NOTIFY iconSizeChanged)
    Q_PROPERTY(int iconSize READ iconSize NOTIFY iconSizeChanged)

public:
    SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer);

    int cellWidth() const;
    int cellHeight() const;
    int iconTextSize() const;
    int iconSize() const;

    QSize thumbnailsSize() const;

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
    int availableWidth() const;
};
