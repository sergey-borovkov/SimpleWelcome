#include "pluginmodel.h"
#include "pluginitem.h"
#include "socialplugin.h"

#include <QtCore/QDebug>

PluginModel::PluginModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

void PluginModel::show(int r)
{
    QVariant v = data(index(r), PluginItem::Item);
    PluginItem *item = v.value<PluginItem *>();

    if(item)
    {
        ISocialPlugin *plugin = item->plugin();
        if(plugin->authorized())
        {
            // need to deauthorize
            item->show();
        }
    }
}
