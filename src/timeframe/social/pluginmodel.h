#ifndef PLUGINMODEl_H
#define PLUGINMODEl_H

#include "../listmodel.h"

class PluginModel : public ListModel
{
public:
    PluginModel(QHash<int, QByteArray> roles, QObject* parent = 0);

public slots:
    void show(int r);
};

#endif // PLUGINMODEl_H
