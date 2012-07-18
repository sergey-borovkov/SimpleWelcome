#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "../listitem.h"

class ISocialPlugin;

class PluginItem : public QObject, public ListItem
{
public:
    enum
    {
        Name,
        Icon,
        Item
    };

    PluginItem(ISocialPlugin *module);
    virtual ~PluginItem() {}
    virtual QString id() const;
    virtual QVariant data(int role) const;
    ISocialPlugin *plugin();
    static const QHash<int,QByteArray> roleNames()
    {
        QHash<int,QByteArray> roles;
        roles.insert(Name, "name");
        roles.insert(Icon, "icon");
        roles.insert(Item, "item");
        return roles;
    }

public slots:
    void show(); // show authentication widget

private:
    ISocialPlugin *m_module;
};

Q_DECLARE_METATYPE(PluginItem *)

#endif // PLUGINITEM_H
