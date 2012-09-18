#include "pluginmodel.h"
#include "pluginitem.h"
#include "socialplugin.h"

#include <QtGui/QWidget>
#include <QtCore/QSettings>

PluginModel::PluginModel(QHash<int, QByteArray> roles, QObject *parent)
    : ListModel(roles, parent)
{
}

void PluginModel::appendRows(const QList<ListItem *> &items)
{
    foreach(ListItem * item, items) {
        PluginItem *plugin = dynamic_cast<PluginItem *>(item);
        connect(plugin, SIGNAL(dataChanged()), SLOT(itemChanged()));
    }

    ListModel::appendRows(items);
}

void PluginModel::logout(int row)
{
    QVariant v = data(index(row), PluginItem::Item);
    PluginItem *item = v.value<PluginItem *>();

    if(item) {
        ISocialPlugin *plugin = item->plugin();
        if(plugin->authorized())
            plugin->requestManager()->logout();
        else {
            // add this plugin to list of enabled plugins
            // should move after logout
            QSettings settings("ROSA", "Timeframe");
            settings.setValue(plugin->name(), 1);

            QWidget *w = plugin->authenticationWidget();
            if(w)
                w->show();
        }
    }
}

void PluginModel::itemChanged()
{
    PluginItem *item = static_cast<PluginItem *>(sender());
    QModelIndex index = indexFromItem(item);
    emit dataChanged(index, index);
}
