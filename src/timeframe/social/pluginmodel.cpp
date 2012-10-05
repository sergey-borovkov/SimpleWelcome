#include "pluginitem.h"
#include "pluginmodel.h"
#include "socialplugin.h"

#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtGui/QWidget>

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

void PluginModel::itemChanged()
{
    PluginItem *item = static_cast<PluginItem *>(sender());
    QModelIndex index = indexFromItem(item);
    emit dataChanged(index, index);
}
