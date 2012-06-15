#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include "../src/timeframe/social/socialplugin.h"

class RequestManager : public ISocialRequestManager
{
    Q_OBJECT    
public:
    explicit RequestManager(QObject *parent = 0);
    virtual void queryWall(const QDate &beginDate, const QDate &endDate);

signals:
    
public slots:
    
};

#endif // REQUESTMANAGER_H
