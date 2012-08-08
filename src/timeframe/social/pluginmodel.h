#ifndef PLUGINMODEl_H
#define PLUGINMODEl_H

#include "../listmodel.h"

class PluginModel : public ListModel
{
    Q_OBJECT
public:
    PluginModel(QHash<int, QByteArray> roles, QObject* parent = 0);
    virtual void appendRows(const QList<ListItem*> &items);
public slots:
    void logout(int row);

private slots:
    void itemChanged();
};

#endif // PLUGINMODEl_H
