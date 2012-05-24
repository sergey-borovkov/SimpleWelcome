#ifndef __sessionprovider_h_
#define __sessionprovider_h_

#include <QObject>
#include <QtDBus/QtDBus>

class SessionProvider : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString searchQuery READ getSearchQuery WRITE setSearchQuery)
public:
    SessionProvider();
    ~SessionProvider();

    void init(void);

public slots:
    void lock(void);
    void shutdown(void);
    //void setSearchQuery(const QString &queryText);
    //QString getSearchQuery(void);

    //signals:
    //void newSearchMatchesFound(void);

private:
    //QString m_searchQuery;

};

#endif // __sessionprovider_h_
