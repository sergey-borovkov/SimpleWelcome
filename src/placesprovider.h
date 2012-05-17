#ifndef __placesprovider_h_
#define __placesprovider_h_

#include <QObject>
#include <QStringList>
#include <QIcon>

namespace Plasma {
  class RunnerManager;
  class QueryMatch;
}

class KDesktopFile;

class PlacesProvider : public QObject
{
  Q_OBJECT
    //Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
public:
  PlacesProvider();
  ~PlacesProvider();

  void init(void);
 
public slots:
  void reload(void);

  QStringList getPlacesList(void);
  void runPlace(const QString &name);
  QIcon getPlaceIcon(const QString &name);
  
  //void setSearchQuery(const QString &queryText);
  //QString getSearchQuery(void);

  //signals:
  //void newSearchMatchesFound(void);
  
private:
  //QString m_searchQuery;

  QStringList m_placesNames;
  QStringList m_placesUrls;
  QList<QIcon> m_placesIcons;
};

#endif // __placesprovider_h_
