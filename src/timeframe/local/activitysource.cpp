#include "activitysource.h"
#include "activityset.h"

#include <QtCore/QDate>


ActivitySource::ActivitySource(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<ActivitySource::Direction>("ActivitySource::Direction");
}
