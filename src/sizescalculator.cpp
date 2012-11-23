#include "sizescalculator.h"

#include "qmlapplicationviewer/qmlapplicationviewer.h"

SizesCalculator::SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer)
    : QObject(parent), viewer(inViewer)
{
    connect(viewer, SIGNAL(availableGeometryChanged()), SIGNAL(iconSizeChanged()));
}

int SizesCalculator::cellWidth() const
{
    return availableHeight() >= 1080 ? 140 :
           availableHeight() >= 1024 ? 130 :
           availableHeight() >= 900 ? 120 :
           availableHeight() >= 850 ? 110 :
           availableHeight() >= 800 ? 110 :
           availableHeight() >= 750 ? 100 :
           70;
}

int SizesCalculator::cellHeight() const
{
    //return (viewer->updatableHeight() - 80 /*topBar*/ - 80 /*bottomBar*/ - (textToGridSpacing() + groupTextHeight())*3 - gridWithGroupsSpacing()*2) / 4 - 1;
    return availableHeight() >= 1080 ? 170 :
           availableHeight() >= 1024 ? 165 :
           availableHeight() >= 900 ? 130 :
           availableHeight() >= 850 ? 120 :
           availableHeight() >= 800 ? 110 :
           availableHeight() >= 750 ? 100 :
           80;
    //return (viewer->updatableHeight() - 162) / 5 - 1;
}

int SizesCalculator::iconTextSize() const
{
    return availableHeight() >= 1080 ? 10 : availableHeight() >= 1024 ? 10 : availableHeight() >= 600 ? 9 : 8;
}

int SizesCalculator::iconSize() const
{
    return availableHeight() >= 1080 ? 96 :
           availableHeight() >= 1024 ? 80 :
           availableHeight() >= 900 ? 64 :
           availableHeight() >= 800 ? 56 :
           availableHeight() >= 768 ? 48 : 32;
}

int SizesCalculator::availableHeight() const
{
     return viewer->availableGeometry().height() + 60;
}
