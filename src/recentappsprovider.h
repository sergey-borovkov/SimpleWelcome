#ifndef __recentappsprovider_h_
#define __recentappsprovider_h_

#include <QObject>
#include <QHash>
#include <QSet>
#include <QIcon>

namespace Plasma {
  class RunnerManager;
  class QueryMatch;
}

class KDesktopFile;

class RecentAppsProvider : public QObject
{
  Q_OBJECT
    //Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
public:
  RecentAppsProvider();
  ~RecentAppsProvider();

  void init(void);
  
public slots:
  void registerLaunchedApp(const QString &name);

  QStringList getRecentAppsList(void);
  void runRecentApp(const QString &name);
  QString getRecentAppIconName(const QString &name);

  //void setSearchQuery(const QString &queryText);
  //QString getSearchQuery(void);

  //signals:
  //void newSearchMatchesFound(void);
  
private:

  void _readRecentApps(void);
  void _writeRecentApps(void);
  void _fillEntries(void);
  QString _fillEntry(const QString &name);
  void _clearEntries(void);
  
  QStringList m_recentAppsList; // Desktopfiles list
  QStringList m_recentAppsEntriesList; // List with names
  QHash<QString, KDesktopFile*> m_recentAppsEntries;
  //QString m_searchQuery;

};

#endif // __recentappsprovider_h_
