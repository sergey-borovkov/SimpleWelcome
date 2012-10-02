#include "pluginitem.h"
#include "socialplugin.h"

#include <QtGui/QWidget>

PluginItem::PluginItem(ISocialPlugin *module)
    : m_module(module)
{
    QObject *object = 0;
    if ((object = dynamic_cast<QObject *>(module))) {
        connect(object, SIGNAL(authorized()), SIGNAL(dataChanged()));
        connect(object, SIGNAL(deauthorized()), SIGNAL(dataChanged()));
    }
}

QString PluginItem::id() const
{
    return QString();
}

QVariant PluginItem::data(int role) const
{
    if (role == Name)
        return m_module->name();
    else if (role == Item) {
        QVariant v;
        v.setValue(const_cast<PluginItem *>(this));
        return v;
    } else if (role == Authorized)
        return m_module->authorized();

    return m_module->icon();
}

bool PluginItem::setData(int role, const QVariant &value)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    return false;
}

ISocialPlugin *PluginItem::plugin()
{
    return m_module;
}
