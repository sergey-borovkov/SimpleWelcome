#pragma once

#include <QDeclarativeImageProvider>

class UserInfoProvider;
class SearchGridModel;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider();

    void setIsLocal(bool isLocal) { m_isLocal = isLocal; }
    void setUserInfoProvider(UserInfoProvider *userInfoProvider) { m_userInfoProvider = userInfoProvider; }
    void setSearchGridModel(SearchGridModel *searchGridModel) { m_searchGridModel = searchGridModel; }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    bool m_isLocal;
    UserInfoProvider *m_userInfoProvider;
    SearchGridModel *m_searchGridModel;
};
