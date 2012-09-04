#pragma once

#include <QDeclarativeImageProvider>

#include <QtCore/QString>

class UserInfoProvider;
class SearchGridModel;

class GeneralIconProvider : public QDeclarativeImageProvider
{
public:
    GeneralIconProvider(QString path_to_assets);

    void setUserInfoProvider(UserInfoProvider *userInfoProvider) {
        m_userInfoProvider = userInfoProvider;
    }
    void setSearchGridModel(SearchGridModel *searchGridModel) {
        m_searchGridModel = searchGridModel;
    }

    QPixmap requestPixmap(const QString &name, QSize *size, const QSize &requestedSize);

private:
    UserInfoProvider *m_userInfoProvider;
    SearchGridModel *m_searchGridModel;
    QString m_pathToAssets;
};
