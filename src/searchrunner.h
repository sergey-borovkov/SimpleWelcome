#ifndef __searchrunner_h_
#define __searchrunner_h_

#include <QObject>
#include <QHash>
#include <QSet>
#include <QIcon>

namespace Plasma {
  class RunnerManager;
  class QueryMatch;
}

class SearchRunner : public QObject
{
  Q_OBJECT
    Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
public:
  SearchRunner();
  ~SearchRunner();

  void init(void);

public slots:
  void newSearchMatches(const QList<Plasma::QueryMatch> &matches);
  void launchSearch(const QString &text);

  void setSearchQuery(const QString &queryText);
  QString getSearchQuery(void);

  QStringList getMatchNames(void);
  QStringList getGroupNames(void);
  QString getMatchGroupName(const QString &name);
  QIcon getMatchIcon(const QString &name);
  void runMatch(const QString &name);
  
signals:
  void newSearchMatchesFound(void);
  
private:
  void _clearMatches(void);

  QString m_searchQuery;
  Plasma::RunnerManager *m_runnerManager;
  QHash<QString, Plasma::QueryMatch*> m_matches;
  QSet<QString> m_groups;
};

#endif // __searchrunner_h_
