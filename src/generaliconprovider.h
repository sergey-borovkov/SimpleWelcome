#pragma once

#include <QDeclarativeImageProvider>

class UserInfoProvider;
class SearchGridModel;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider();

    void setUserInfoProvider(UserInfoProvider *userInfoProvider) { m_userInfoProvider = userInfoProvider; }
    void setSearchGridModel(SearchGridModel *searchGridModel) { m_searchGridModel = searchGridModel; }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    UserInfoProvider *m_userInfoProvider;
    SearchGridModel *m_searchGridModel;
};
