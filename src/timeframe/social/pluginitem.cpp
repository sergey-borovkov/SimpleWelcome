#include "pluginitem.h"
#include "socialplugin.h"

#include <QtGui/QWidget>

PluginItem::PluginItem(ISocialPlugin *module)
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
    else if(role == Item)
    {
        QVariant v;
        v.setValue(const_cast<PluginItem *>(this));
        return v;
    }
    else if(role == Authorized)
           return m_module->authorized();

    return m_module->icon();
}

ISocialPlugin *PluginItem::plugin()
{
    return m_module;
}
