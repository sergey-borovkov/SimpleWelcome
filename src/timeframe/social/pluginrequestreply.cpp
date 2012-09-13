#include "pluginrequestreply.h"
#include "socialplugin.h"

PluginRequestReply::PluginRequestReply(Request *request, const QString &sourceId, QObject *parent) :
    QObject(parent),
    m_finished(false),
    m_sourceId(sourceId)
{
    QObject *r = dynamic_cast<QObject *>(request);
    if(r != 0) {
        connect(r, SIGNAL(success()), SLOT(requestSuccess()));
        connect(r, SIGNAL(newItemId(QString)), SLOT(newItemId(QString)));
        connect(r, SIGNAL(error(QString)), SLOT(error(QString)));
    }
}

bool PluginRequestReply::isFinished() const
{
    return m_finished;
}

QString PluginRequestReply::id() const
{
    return m_id;
}

QString PluginRequestReply::errorString() const
{
    return m_errorString;
}

QString PluginRequestReply::sourceId()
{
    return m_sourceId;
}

void PluginRequestReply::newItemId(QString id)
{
    m_id = id;
}

void PluginRequestReply::error(QString error)
{
    m_errorString = error;
    m_finished = true;
    emit failure(this);
    emit finished();
}
void PluginRequestReply::requestSuccess()
{
    m_finished = true;   
    emit success(this);
    emit finished();
}
