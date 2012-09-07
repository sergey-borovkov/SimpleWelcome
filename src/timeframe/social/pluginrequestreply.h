#ifndef PLUGINREQUESTREPLY_H
#define PLUGINREQUESTREPLY_H

#include <QtCore/QObject>

class Request;

/**
 * @brief The PluginRequestReply class encapsulates replies from social plugins
 *        and allows to read those replies from QML. This class is necessary because
 *        Request is interface and can not be used from QML (and it does not have signals
 *        plugins are supposed to emit for the same reasons).
 */
class PluginRequestReply : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool finished READ isFinished NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString)
public:
    /**
     * @brief PluginReply
     * @param request Pointer to Request
     * @param parent
     */
    explicit PluginRequestReply(Request *request, QObject *parent = 0);

    /**
     * @brief isComplete Check if request already completed
     * @return bool
     */
    Q_INVOKABLE bool isFinished() const;

    /**
     * @brief Return new item's id if it was created
     * @return id  Id string or empty string if no new objects were created
     */
    Q_INVOKABLE QString id() const;

    /**
     * @brief errorString
     * @return last error string
     */
    Q_INVOKABLE QString errorString() const;

signals:
    void finished();

private slots:
    void requestSuccess();
    void newItemId(QString id);
    void error(QString error);

private:
    QString m_id;
    QString m_errorString;
    bool m_finished;
};

#endif // PLUGINREQUESTREPLY_H
