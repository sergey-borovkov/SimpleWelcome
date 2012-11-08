#include "pluginrequestreply.h"
#include "socialplugin.h"

PluginRequestReply::PluginRequestReply(Request *request, const QString &sourceId, const QString &pluginName, QObject *parent)
    : QObject(parent)
    , m_sourceId(sourceId)
    , m_pluginName(pluginName)
    , m_finished(false)
{
    QObject *r = dynamic_cast<QObject *>(request);
    if (r != 0) {
        connect(r, SIGNAL(success()), SLOT(requestSuccess()));
        connect(r, SIGNAL(newItemId(QString)), SLOT(newItemId(QString)));
        connect(r, SIGNAL(gotUserPictureUrl(QString, QString)), SLOT(gotUserPictureUrl(QString, QString)));
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

QString PluginRequestReply::userPictureUrl() const
{
    return m_userPictureUrl;
}

QString PluginRequestReply::userId() const
{
    return m_userId;
}

QString PluginRequestReply::userName() const
{
    return m_userName;
}

QString PluginRequestReply::audioId() const
{
    return m_audioId;
}

QString PluginRequestReply::audioOwnerId() const
{
    return m_audioOwnerId;
}

QString PluginRequestReply::audioUrl() const
{
    return m_audioUrl;
}

QString PluginRequestReply::videoId() const
{
    return m_videoId;
}

QString PluginRequestReply::videoOwnerId() const
{
    return m_videoOwnerId;
}

QString PluginRequestReply::videoUrl() const
{
    return m_videoUrl;
}

QString PluginRequestReply::pluginName() const
{
    return m_pluginName;
}

void PluginRequestReply::newItemId(QString id)
{
    m_id = id;
}

void PluginRequestReply::gotUserPictureUrl(QString id, QString url)
{
    m_userId = id;
    m_userPictureUrl = url;
}

void PluginRequestReply::gotAudioUrl(QString aid, QString ownerId, QString url)
{
    m_audioId = aid;
    m_audioOwnerId = ownerId;
    m_audioUrl = url;
}

void PluginRequestReply::gotVideoUrl(QString vid, QString ownerId, QString url)
{
    m_videoId = vid;
    m_videoOwnerId = ownerId;
    m_videoUrl = url;
}

void PluginRequestReply::gotUserName(QString id, QString name)
{
    m_userId = id;
    m_userName = name;
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
