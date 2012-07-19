#pragma once

#include <QHash>
#include <QSet>
#include <QIcon>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>

namespace Plasma
{
    class RunnerManager;
    class QueryMatch;
}

class SearchGridModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)


public:
    enum
    {
        CaptionRole = Qt::UserRole + 1,
        ImagePathRole,
        GroupRole
    };

    explicit SearchGridModel(QObject* parent = 0);
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;




public slots:
    void newSearchMatches(const QList<Plasma::QueryMatch> &matches);
    void launchSearch(const QString &text);

    void setSearchQuery(const QString &queryText);
    QString getSearchQuery();

    QStringList getMatchNames();
    QStringList getGroupNames();
    QString getMatchGroupName(const QString &name) const;
    QIcon getMatchIcon(const QString &name);
    void runMatch(const QString &name);

signals:
    void newSearchMatchesFound();
    void currentTabIndexChanged(int newCurrentIndex);

private:
    void _clearMatches();

    QString m_searchQuery;
    Plasma::RunnerManager *m_runnerManager;
    QHash<QString, Plasma::QueryMatch*> m_matches;
    QSet<QString> m_groups;
};


class SearchFilterGridModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString group READ getGroup WRITE setGroup)

public:
    SearchFilterGridModel(QObject* parent = NULL, QAbstractListModel *sourceModel = NULL);

    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

    QString getGroup() { return group; }
    void setGroup(QString newGroup) { group = newGroup; }

public slots:
    void itemClicked(int newIndex);

private:
    QString group;
};
