#ifndef __appprovider_h_
#define __appprovider_h_

#include "appentity.h"

#include <KServiceGroup>
#include <KService>

class AppProvider : public QObject
{
  Q_OBJECT
public:
    AppProvider();
    ~AppProvider();

    void init(void);

public Q_SLOTS:
    Q_INVOKABLE AppEntity *getEntity(const QString &name) { return m_appEntities[name]; };
    Q_INVOKABLE void runEntity(const QString &name);
    QStringList getEntityNames(void);
    QStringList getRootGroups(void);
    
    
 private:
    QHash<QString, AppEntity*> m_appEntities;
    QStringList m_rootGroups;

    void _deepExtract(KServiceGroup *group);

    void _printDebug(void);
};

#endif // __appprovider_h_
