#ifndef __userinfoprovider_h_
#define __userinfoprovider_h_

#include <QObject>
#include <KUser>

class UserInfoProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullName READ getFullName)
    Q_PROPERTY(QString loginName READ getLoginName)
    Q_PROPERTY(QString userName READ getUserName)
public:
    UserInfoProvider();
    ~UserInfoProvider();

    void init(void);

    QString getFullName(void);
    QString getLoginName(void);
    QString getUserName(void);

    QString getIconPath(void);

public slots:
    //void setSearchQuery(const QString &queryText);
    //QString getSearchQuery(void);

    //signals:
    //void newSearchMatchesFound(void);

private:
    //QString m_searchQuery;

    KUser *m_userInfo;
};

#endif // __userinfoprovider_h_
