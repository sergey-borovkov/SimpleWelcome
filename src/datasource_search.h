#pragma once

#include <QHash>
#include <QSet>
#include <QIcon>
#include "appitem.h"
#include "datasource.h"

namespace Plasma
{
class RunnerManager;
class QueryMatch;
}

struct MatchResults {
    QString group;
    QString name;
    Plasma::QueryMatch* plasmaMatch;
};

class DataSource_RecentApps;

class DataSource_Search : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)


public:
    explicit DataSource_Search(QObject* parent = 0, DataSource_RecentApps *recentApps = 0);
    Q_INVOKABLE virtual int getItemCount(QString group = "");
    Q_INVOKABLE QString itemUrlDnd(int id, QString group);
    QList<QPair<QString, QString> > getRunnersNames();


public slots:
    void newSearchMatches(const QList<Plasma::QueryMatch> &newMatches);
    void launchSearch(const QString &text);

    void setSearchQuery(const QString &queryText);
    QString getSearchQuery();

    QIcon getMatchIcon(const QString &name);


    virtual void itemClicked(int newIndex, QString group);
    virtual void getContent();


    virtual void test2();

signals:
    void newItemData(QVariantMap itemData);
    void resetContent();
    void runDesktopFile(QString desktopFile);


    void currentTabIndexChanged(int newCurrentIndex);

private:
    QString m_searchQuery;
    Plasma::RunnerManager *m_runnerManager;

    QVector<MatchResults> matches;
    //QHash<QString, Plasma::QueryMatch*> m_matches;
    QSet<QString> m_groups;

    DataSource_RecentApps *recentApps;
};
