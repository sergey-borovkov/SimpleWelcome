#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "../listitem.h"

class ISocialModule;

class PluginItem : public ListItem
{
public:
    enum {
        Name,
        Icon
    };

    PluginItem(const ISocialModule *module);
    virtual ~PluginItem() {}
    virtual QString id() const;
    virtual QVariant data(int role) const;

    static const QHash<int,QByteArray> roleNames()
    {
        QHash<int,QByteArray> roles;
        roles.insert(Name, "name");
        roles.insert(Icon, "icon");
        return roles;
    }

private:
    const ISocialModule *m_module;
};

#endif // PLUGINITEM_H
