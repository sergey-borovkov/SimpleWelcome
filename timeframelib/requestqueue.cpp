#include "requestqueue.h"

#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtCore/QHash>

RequestQueue::RequestQueue()
    : m_requestsPerSecond(3), m_started(false)
{
    requestCountTimestamp.time = 0;
    requestCountTimestamp.requestCount = 0;
}

RequestQueue *RequestQueue::instance(const QString &pluginName)
{
    static QHash<QString, RequestQueue *> requestQueueHash;
    QHash<QString, RequestQueue *>::const_iterator it = requestQueueHash.constFind(pluginName);

    if (it != requestQueueHash.constEnd()) {
        return it.value();
    }

    RequestQueue *requestQueue = new RequestQueue();
    requestQueueHash.insert(pluginName, requestQueue);
    return requestQueue;
}

void RequestQueue::enqueue(Request *request, Request::Priority priority)
{
    if (!request)
        return;

    m_requestQueue.push(std::make_pair(priority, request));

    if (!m_started)
        QTimer::singleShot(0, this, SLOT(start()));
}

void RequestQueue::setMaximumRequestsPerSecond(int amount)
{
    m_requestsPerSecond = amount;
}

void RequestQueue::start()
{
    const uint time = QDateTime::currentDateTime().toTime_t();
    int requests = 0;
    if (time == requestCountTimestamp.time) {
        const int delta = m_requestsPerSecond - requestCountTimestamp.requestCount;
        if (delta > 0) {
            requests = qMin(delta, static_cast<int>(m_requestQueue.size()));
            requestCountTimestamp.requestCount += requests;
        }
    }
    else {
        requests = qMin(m_requestsPerSecond, static_cast<int>(m_requestQueue.size()));
        requestCountTimestamp.time = time;
        requestCountTimestamp.requestCount = requests;
    }

    for (int i = 0; i < requests; i++) {
        PriorityRequest request = m_requestQueue.top();
        request.second->run();
        m_requestQueue.pop();
    }

    if (m_requestQueue.size()) {
        // we can make limited amount of requests per second
        // just to be sure let's make new requests after 1.1 seconds
        QTimer::singleShot(1100, this, SLOT(start()));
        m_started = true;
    }
    else {
        m_started = false;
    }
}
