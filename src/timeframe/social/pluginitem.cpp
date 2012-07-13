#include "pluginitem.h"
#include "socialplugin.h"

PluginItem::PluginItem(const ISocialModule *module)
    : m_module(module)
{
}

QString PluginItem::id() const
{
    return QString();
}

QVariant PluginItem::data(int role) const
{
    if(role == Name)
        return m_module->name();

    return m_module->icon();
}
