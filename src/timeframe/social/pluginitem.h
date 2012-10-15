#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

#include <listitem.h>

class ISocialPlugin;

class PluginItem : public QObject, public ListItem
{
    Q_OBJECT
public:
    enum {
        Name,
        Icon,
        Item,
        Authorized
    };

    PluginItem(ISocialPlugin *module);
    virtual ~PluginItem() {}
    virtual QString id() const;
    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);

    ISocialPlugin *plugin();
    static const QHash<int, QByteArray> roleNames() {
        QHash<int, QByteArray> roles;
        roles.insert(Name, "name");
        roles.insert(Icon, "icon");
        roles.insert(Item, "item");
        roles.insert(Authorized, "authorized");
        return roles;
    }

signals:
    void dataChanged();

private:
    ISocialPlugin *m_module;
};

Q_DECLARE_METATYPE(PluginItem *)

#endif // PLUGINITEM_H
