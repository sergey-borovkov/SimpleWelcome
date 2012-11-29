#include "sizescalculator.h"

#include "qmlapplicationviewer/qmlapplicationviewer.h"

SizesCalculator::SizesCalculator(QObject *parent, QmlApplicationViewer *inViewer)
    : QObject(parent), viewer(inViewer)
{
    connect(viewer, SIGNAL(availableGeometryChanged()), SIGNAL(iconSizeChanged()));
}

int SizesCalculator::cellWidth() const
{
    return availableWidth() >= 1920 ? 140 :
           availableWidth() >= 1440 ? 130 :
           availableWidth() >= 1280 && availableHeight() < 1024 ? 120 : availableWidth() >= 1280 ? 130 :
           availableWidth() >= 1024 ? 112 :
           availableWidth() >= 800 ? 102 :
           86;
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
    return availableWidth() >= 1920 ? 10 :
           availableWidth() >= 1280 && availableHeight() >= 1024 ? 10 :
           availableWidth() >= 1024 && availableHeight() >= 768 ? 9 :
           availableWidth() >= 800 && availableHeight() >= 600 ? 8 :
           7;
}

int SizesCalculator::iconSize() const
{
    return availableHeight() >= 1080 ? 96 :
           availableHeight() >= 1024 ? 80 :
           availableHeight() >= 900 ? 64 :
           availableHeight() >= 800 ? 56 :
           availableHeight() >= 720 ? 48 :
                                      32;
}

QSize SizesCalculator::thumbnailsSize() const
{
    QSize res;
    res.setWidth(availableWidth() >= 1920 ? 120 :
                 availableWidth() >= 1440 ? 106 :
                 availableWidth() >= 1280 ? 106 :
                 availableWidth() >= 1024 ? 96 :
                 availableWidth() >=  800 ? 80 :
                                            32);
    res.setHeight(availableHeight() >= 1080 ? 100 :
                  availableHeight() >= 1024 ? 88 :
                  availableHeight() >=  900 ? 64 :
                  availableHeight() >=  800 ? 56 :
                  availableHeight() >=  720 ? 48 :
                                              32);
    return res;
}

int SizesCalculator::availableHeight() const
{
     return viewer->availableGeometry().height() + 60;
}

int SizesCalculator::availableWidth() const
{
     return viewer->availableGeometry().width();
}
