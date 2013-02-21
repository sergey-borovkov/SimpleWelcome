#ifndef REQUESTQUEUE_H
#define REQUESTQUEUE_H

#include <QtCore/QObject>
#include <QtCore/QTime>
#include <queue>
#include <socialplugin.h>

#include "timeframelib_global.h"

class TIMEFRAMELIB_EXPORT RequestQueue : public QObject
{
    Q_OBJECT
public:
    RequestQueue(QObject *parent = 0);
    void enqueue(Request *request, Request::Priority priority = Request::Normal);

    void setMaximumRequestsPerSecond(int amount);

private slots:
    void start();

private:
    typedef std::pair<int, Request*> PriorityRequest;

    struct {
        uint time;
        int requestCount;
    } requestCountTimestamp; // keep amount of requests made in last query

    std::priority_queue<PriorityRequest> m_requestQueue;
    int m_requestsPerSecond;
    bool m_started;
};

#endif // REQUESTQUEUE_H
