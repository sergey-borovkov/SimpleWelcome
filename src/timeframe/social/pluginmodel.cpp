#include "pluginmodel.h"
#include "pluginitem.h"
#include "socialplugin.h"

#include <QtCore/QDebug>
#include <QtGui/QWidget>

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
            plugin->requestManager()->logout();
        else
        {
            QWidget *w = plugin->authenticationWidget();
            if(w)
                w->show();
        }
    }
}
