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

struct MatchResults
{
    QString group;
    QString name;
    Plasma::QueryMatch* plasmaMatch;
};

class SearchGridModel : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)


public:
    explicit SearchGridModel(QObject* parent = 0);
    Q_INVOKABLE virtual int getItemCount(QString group = "");



public slots:
    void newSearchMatches(const QList<Plasma::QueryMatch> &newMatches);
    void launchSearch(const QString &text);

    void setSearchQuery(const QString &queryText);
    QString getSearchQuery();

    QIcon getMatchIcon(const QString &name);
    void runMatch(const QString &name);



    virtual void itemClicked(int newIndex);
    virtual void getContent();


    virtual void test2();

signals:
    void newItemData(QString iconPath, QString name, int id, QString group);
    void resetContent();



    void newSearchMatchesFound();
    void currentTabIndexChanged(int newCurrentIndex);

private:
    QString m_searchQuery;
    Plasma::RunnerManager *m_runnerManager;

    QVector<MatchResults> matches;
    //QHash<QString, Plasma::QueryMatch*> m_matches;
    QSet<QString> m_groups;
};
