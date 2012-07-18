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
    return m_module->icon();
}

ISocialPlugin *PluginItem::plugin()
{
    return m_module;
}

void PluginItem::show()
{
    QWidget *w = const_cast<ISocialPlugin *>(m_module)->authenticationWidget();
    if(w)
        w->show();
}
