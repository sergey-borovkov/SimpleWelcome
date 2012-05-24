#ifndef __documentsprovider_h_
#define __documentsprovider_h_

#include <QObject>
#include <QStringList>
#include <QIcon>
#include <QHash>
#include <QStringList>

namespace Plasma {
class RunnerManager;
class QueryMatch;
}

class KDesktopFile;

class DocumentsProvider : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
public:
    DocumentsProvider();
    ~DocumentsProvider();

    void init(void);

public slots:
    void reload(void);

    QStringList getDocsList(void);
    void runDoc(const QString &name);
    QString getDocIconName(const QString &name);

    //void setSearchQuery(const QString &queryText);
    //QString getSearchQuery(void);

    //signals:
    //void newSearchMatchesFound(void);

private:
    //QString m_searchQuery;
    void _clearEntries(void);

    QStringList m_recentDocsList; // Desktopfiles list
    QHash<QString, KDesktopFile*> m_recentDocsEntries;
};

#endif // __documentsprovider_h_
